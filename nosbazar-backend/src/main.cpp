#include "net.h"
#include <spdlog/spdlog.h>
#include "auth/nosauth.h"
#include "dotenv.h"
#include <asio.hpp>
#include <random/random.h>
#include <login_packet.h>
#include <strings/hex.h>
#include <nosclient.h>
#include "env.h"

void handle_nstest(const std::string packet) {
	SPDLOG_DEBUG(packet);
}


int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	nosbazar::nosclient::check_and_download_outdated_files();

	Env env;

	auto identity = std::make_shared<nosbazar::auth::Identity>(env.identity_path);
	nosbazar::auth::NosAuth nosauth(identity);
	auto result = nosauth.authenticate({ .email = env.gf_email, .password = env.gf_password});

	auto accs = nosauth.get_accounts();

	std::string account_id = env.account_id;

	std::optional<std::string> token;

	if (accs.contains(account_id)) {
		token = nosauth.get_session_token(account_id);
	}
	else {
		SPDLOG_ERROR("Could not find account");
	}

	asio::io_context context;

	auto client = std::make_unique<nosbazar::net::TCPClient>(context);
	client->connect("79.110.84.75", 4000);

	nosbazar::net::LoginSession login_session(std::move(client));

	login_session.subscribe("NsTeST", handle_nstest);

	std::string hex_token = nosbazar::strings::hexlify(token.value_or(""));

	std::string client_version = nosbazar::nosclient::get_nostale_client_version();

	std::string md5 = nosbazar::nosclient::get_md5_for_login_packet();

	nosbazar::packets::login::NoS0577Packet packet(hex_token, "NONE_CII", "0", client_version, md5);

	std::string login_packet = packet.string();

	login_session.send(login_packet);

	context.run();
}

