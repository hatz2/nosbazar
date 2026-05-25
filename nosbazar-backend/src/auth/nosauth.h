#pragma once

#include <string>
#include <map>
#include <optional>
#include <cstdint>
#include <vector>
#include "identity.h"
#include <nlohmann/json.hpp>

namespace nosbazar::auth {
	class NosAuth {
	public:
		NosAuth(std::shared_ptr<Identity> identity);
		NosAuth(std::shared_ptr<Identity> identity, std::string token);

		struct AuthParams {
			std::string email;
			std::string password;
		};

		enum class AuthResult {
			invalid_credentials,
			captcha,
			unknown_error,
			ok
		};

		AuthResult authenticate(const AuthParams& params);

		[[nodiscard]] nlohmann::json get_accounts() const;

		std::optional<std::string> get_session_token(const std::string& account_id) const;

	private:
		struct UserInfo {
			std::string gf_account_id;
			std::string locale;
			std::string email;
		};

		UserInfo get_user_information();

		bool send_iovation(const std::string& account_id) const;

		std::optional<char> get_first_number(std::string_view uuid) const;

		std::string generate_third_type_user_agent_magic(const std::string& account_id) const;

		void init_gf_version();
		void init_all_certs();
		void init_game_session_id();
		void init_cert();

		static constexpr std::string_view game_id = "dd4e22d6-00d1-44b9-8126-d8b40e0cd7c9";
		std::string locale = "en-US";
		std::string browser_user_agent = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36";
		std::string installation_id = "f4bca3e5-8e1b-4aed-9ea5-5e771cf903bd";
		std::string all_certs;
		std::string gf_version;
		std::string chrome_version;
		std::string token;
		std::string game_session_id;
		std::string cert;
		UserInfo user_info;
		std::shared_ptr<Identity> identity;
	};
}