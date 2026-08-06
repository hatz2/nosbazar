#pragma once

#include "dotenv.h"
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

bool is_dev_mode()
{
	const char* override = std::getenv("NOSBAZAR_DEV");
	if (override != nullptr) {
		return std::string(override) == "1";
	}

#ifdef NOSBAZAR_DEV
	return true;
#else
	return false;
#endif
}

std::string get_required_env(const char* key, std::string_view fallback = {})
{
	const char* value = std::getenv(key);
	if (value == nullptr) {
		if (!fallback.empty()) {
			return std::string(fallback);
		}
		throw std::runtime_error(std::string("missing required env var: ") + key);
	}
	return value;
}

} // anonymous namespace

struct Env {
	std::string identity_path;
	std::string gf_email;
	std::string gf_password;
	std::string installation_id;
	std::string login_server_ip;
	uint16_t login_server_port;
	uint16_t max_clients;

	Env() {
		if (is_dev_mode()) {
			dotenv::init();
		}
		identity_path = get_required_env("IDENTITY_PATH");
		gf_email = get_required_env("GF_EMAIL");
		gf_password = get_required_env("GF_PASSWORD");
		installation_id = get_required_env("INSTALLATION_ID");
		login_server_ip = get_required_env("LOGIN_SERVER_IP");
		login_server_port = std::stoi(get_required_env("LOGIN_SERVER_PORT"));
		max_clients = std::stoi(get_required_env("MAX_CLIENTS", "5"));
	}
};
