#include "nosauth.h"
#include <openssl/sha.h>
#include <crow/json.h>
#include "net.h"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <ranges>
#include "blackbox.h"
#include <crypto/uuid.h>
#include <crypto/hash.h>
#include <fstream>
#include <env.h>
#include "token_repository.h"

using json = nlohmann::json;

namespace {
	constexpr int JSON_INDENT = 4;
}

nosbazar::auth::NosAuth::NosAuth(std::shared_ptr<Identity> identity) : NosAuth(identity, {})
{

}

nosbazar::auth::NosAuth::NosAuth(std::shared_ptr<Identity> identity, std::string token) 
	: identity(std::move(identity))
	, token(std::move(token))
{
	init_gf_version();
	init_all_certs();
	init_game_session_id();
	init_cert();
	init_installation_id();
	
}

nosbazar::auth::NosAuth::AuthResult nosbazar::auth::NosAuth::authenticate(const AuthParams& params)
{
	std::string_view url = "https://spark.gameforge.com/api/v2/authProviders/credentials/sessions";
	
	std::vector<std::string> headers = {
		"Content-Type: application/json",
		fmt::format("User-Agent: {}", browser_user_agent),
		fmt::format("tnt-installation-id: {}", installation_id),
		fmt::format("gf-installation-id: {}", installation_id),
	};

	json body = {
		{"blackbox", create_blackbox()},
		{"email", params.email},
		{"locale", locale},
		{"password", params.password}
	};

	auto result = net::post(url, body.dump(), headers);

	if (result) {
		auto json_response = json::parse(result->body);

		SPDLOG_TRACE("authenticate response: {}", json_response.dump(JSON_INDENT));

		if (result->status_code == 409) {
			const auto& error_types = json_response["errorTypes"];

			if (error_types.contains("CREDENTIALS_INVALID")) {
				return AuthResult::invalid_credentials;
			}

			if (error_types.contains("CHALLENGE_REQUIRED")) {
				return AuthResult::captcha;
			}

			return AuthResult::unknown_type_of_captcha;
		}

		if (result->status_code == 403) {
			return AuthResult::invalid_credentials;
		}
		
		if (result->status_code == 201) {
			token = json_response["token"];
			TokenRepository::instance().add_token(params.email, token);
			TokenRepository::instance().save();
			return AuthResult::ok;
		}

		SPDLOG_ERROR("Unknown status code: {}", result->status_code);

		return AuthResult::unknown_error;
	}
	else {
		SPDLOG_ERROR("Curl error: {}", fmt::underlying(result.error()));
		return AuthResult::unknown_error;
	}
}

json nosbazar::auth::NosAuth::get_accounts()
{
	if (token.empty()) {
		return {};
	}

	if (!get_user_information()) {
		return {};
	}

	std::string_view url = "https://spark.gameforge.com/api/v1/user/accounts";

	std::vector<std::string> headers = {
		fmt::format("User-Agent: {}", browser_user_agent),
		fmt::format("TNT-Installation-Id: {}", installation_id),
		fmt::format("Authorization: Bearer {}", token),
	};

	auto result = net::get(url, headers);

	if (result) {
		SPDLOG_TRACE("get_accounts response: {}", json::parse(result->body).dump(JSON_INDENT));

		if (result->status_code != 200) {
			return {};
		}
		
		return json::parse(result->body);
	}
	else {
		return {};
	}
}

bool nosbazar::auth::NosAuth::get_user_information()
{
	if (token.empty()) {
		return false;
	}

	std::string_view url = "https://spark.gameforge.com/api/v1/user/me";

	std::vector<std::string> headers = {
		fmt::format("User-Agent: {}", browser_user_agent),
		fmt::format("TNT-Installation-Id: {}", installation_id),
		fmt::format("Authorization: Bearer {}", token),
	};

	auto result = net::get(url, headers);

	if (!result || result->status_code != 200) {
		return false;
	}

	auto response = json::parse(result->body);
	user_info.gf_account_id = response["id"].get<std::string>();
	user_info.locale = response.value("locale", locale);
	user_info.email = response.value("email", std::string());

	return true;
}

bool nosbazar::auth::NosAuth::send_iovation(const std::string& account_id) const
{
	std::string_view url = "https://spark.gameforge.com/api/v1/auth/iovation";

	std::vector<std::string> headers = {
		"Content-Type: application/json",
		fmt::format("User-Agent: {}", browser_user_agent),
		fmt::format("TNT-Installation-Id: {}", installation_id),
		fmt::format("Authorization: Bearer {}", token),
	};

	json content = {
		{"accountId", account_id},
		{"blackbox", create_blackbox()},
		{"type", "play_now"}
	};

	auto reply = net::post(url, content.dump(), headers);

	
	if (!reply) {
		return false;
	}

	SPDLOG_TRACE("send_iovation response: {}", json::parse(reply->body).dump(JSON_INDENT));

	if (reply->status_code != 200) {
		return false;
	}

	auto response = json::parse(reply->body);

	return response["status"] == "ok";
}

std::optional<std::string> nosbazar::auth::NosAuth::get_session_token(const std::string& account_id)
{
	std::lock_guard<std::mutex> lock(session_mutex);

	std::string_view url = "https://spark.gameforge.com/api/v1/auth/thin/codes";

	if (token.empty()) {
		return std::nullopt;
	}

	init_game_session_id();

	if (!send_iovation(account_id)) {
		return std::nullopt;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::vector<std::string> headers = {
		"Content-Type: application/json",
		fmt::format("User-Agent: Chrome/{} ({})", chrome_version, generate_third_type_user_agent_magic(account_id)),
		fmt::format("Authorization: Bearer {}", token),
		fmt::format("tnt-installation-id: {}", installation_id)
	};

	std::string gsid = fmt::format("{}-{}", game_session_id, random::random_int(1000, 9999));
	identity->update();
	EncryptedBlackbox blackbox(identity, account_id, gsid, installation_id);

	json content = {
		{"platformGameAccountId", account_id},
		{"gsid", gsid},
		{"blackbox", blackbox.encrypted()},
		{"gameId", game_id}
	};

	auto reply = net::post(url, content.dump(4), headers);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	if (!reply) {
		return std::nullopt;
	}

	SPDLOG_TRACE("get_session_token response: {}", json::parse(reply->body).dump(JSON_INDENT));

	if (reply->status_code != 201) {
		return std::nullopt;
	}

	auto response = json::parse(reply->body);

	if (response.contains("code")) {
		return response["code"];
	}
	else {
		return std::nullopt;
	}
}

void nosbazar::auth::NosAuth::set_login_token(const std::string& token)
{
	this->token = token;
}

std::string nosbazar::auth::NosAuth::create_blackbox() const
{
	identity->update();
	identity->set_request(nullptr);
	Blackbox blackbox(identity);
	return blackbox.encoded();
}

std::optional<char> nosbazar::auth::NosAuth::get_first_number(std::string_view uuid) const
{
	for (const auto& c : uuid) {
		if (std::isdigit(c)) {
			return c;
		}
	}

	return std::nullopt;
}

std::string nosbazar::auth::NosAuth::generate_third_type_user_agent_magic(const std::string& account_id) const
{
	std::string cert_hash, version_hash, installation_id_hash, account_id_hash, sum_hash;
	auto first_number = get_first_number(installation_id);
	std::string first_two_letters = account_id.substr(0, 2);

	if (!first_number.has_value() || (first_number.value() - '0') % 2 == 0) {
		cert_hash = crypto::sha256(cert);
		version_hash = crypto::sha1(chrome_version);
		installation_id_hash = crypto::sha256(installation_id);
		account_id_hash = crypto::sha1(account_id);
		sum_hash = crypto::sha256(cert_hash + version_hash + installation_id_hash + account_id_hash);
		
		return first_two_letters + sum_hash.substr(0, 8);
	}
	else {
		cert_hash = crypto::sha1(cert);
		version_hash = crypto::sha256(chrome_version);
		installation_id_hash = crypto::sha1(installation_id);
		account_id_hash = crypto::sha256(account_id);
		sum_hash = crypto::sha256(cert_hash + version_hash + installation_id_hash + account_id_hash);

		return first_two_letters + sum_hash.substr(sum_hash.size() - 8);
	}
}

void nosbazar::auth::NosAuth::init_gf_version()
{
	std::string_view url = "http://dl.tnt.gameforge.com/tnt/final-ms3/clientversioninfo.json";

	auto reply = net::get(url);

	if (!reply) {
		return;
	}

	if (reply->status_code != 200) {
		return;
	}

	auto response = json::parse(reply->body);

	gf_version = response["minimumVersionForDelayedUpdate"];
	chrome_version = "C" + response["version"].get<std::string>();
}

void nosbazar::auth::NosAuth::init_all_certs()
{
}

void nosbazar::auth::NosAuth::init_game_session_id()
{
	game_session_id = crypto::generate_uuid_v4();
}

void nosbazar::auth::NosAuth::init_cert()
{
	std::ifstream file("./assets/cert.pem");
	std::ostringstream stream;
	stream << file.rdbuf();
	cert = stream.str();
}

void nosbazar::auth::NosAuth::init_installation_id()
{
	Env env;
	installation_id = env.installation_id;
}
