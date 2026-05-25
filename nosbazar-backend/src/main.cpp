#include "net.h"
#include <spdlog/spdlog.h>
#include "auth/nosauth.h"
#include "dotenv.h"

struct Env {
	std::string identity_path;
	std::string gf_email;
	std::string gf_password;
	std::string account_id;

	Env() {
		dotenv::init();

		identity_path = dotenv::get("IDENTITY_PATH").value();
		gf_email = dotenv::get("GF_EMAIL").value();
		gf_password = dotenv::get("GF_PASSWORD").value();
		account_id = dotenv::get("ACCOUNT_ID").value();
	}
};


int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	Env env;

	auto identity = std::make_shared<nosbazar::auth::Identity>(env.identity_path);
	nosbazar::auth::NosAuth nosauth(identity);
	auto result = nosauth.authenticate({ .email = env.gf_email, .password = env.gf_password});

	auto accs = nosauth.get_accounts();

	std::string account_id = env.account_id;

	if (accs.contains(account_id)) {
		auto token = nosauth.get_session_token(account_id);

		if (token)
			SPDLOG_DEBUG(token.value());
	}
	else {
		SPDLOG_ERROR("No account!!");
	}
}

