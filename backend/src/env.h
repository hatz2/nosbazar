#pragma once

#include "dotenv.h"
#include <string>

struct Env {
	std::string identity_path;
	std::string gf_email;
	std::string gf_password;
	std::string installation_id;
	std::string login_server_ip;
	uint16_t login_server_port;
	uint16_t max_clients;

	Env() {
		if (!initialized) {
			dotenv::init();
			initialized = true;
		}

		identity_path = dotenv::get("IDENTITY_PATH").value();
		gf_email = dotenv::get("GF_EMAIL").value();
		gf_password = dotenv::get("GF_PASSWORD").value();
		installation_id = dotenv::get("INSTALLATION_ID").value();
		login_server_ip = dotenv::get("LOGIN_SERVER_IP").value();
		login_server_port = std::stoi(dotenv::get("LOGIN_SERVER_PORT").value());
		max_clients = std::stoi(dotenv::get("MAX_CLIENTS").value_or("5"));
	}

private:
	static inline bool initialized = false;
};