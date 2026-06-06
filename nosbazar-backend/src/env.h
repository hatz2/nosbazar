#pragma once

#include "dotenv.h"
#include <string>

struct Env {
	std::string identity_path;
	std::string gf_email;
	std::string gf_password;
	std::string account_id;
	std::string installation_id;

	Env() {
		if (!initialized) {
			dotenv::init();
			initialized = true;
		}

		identity_path = dotenv::get("IDENTITY_PATH").value();
		gf_email = dotenv::get("GF_EMAIL").value();
		gf_password = dotenv::get("GF_PASSWORD").value();
		account_id = dotenv::get("ACCOUNT_ID").value();
		installation_id = dotenv::get("INSTALLATION_ID").value();
	}

private:
	static inline bool initialized = false;
};