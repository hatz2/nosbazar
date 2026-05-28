#pragma once

#include <string>

namespace nosbazar::packets::login {
	class NoS0577Packet {
	public:
		explicit NoS0577Packet(std::string session_token, std::string installation_id, std::string region_code, std::string client_version, std::string client_hash);

		std::string string();
	private:
		std::string session_token;
		std::string installation_id;
		std::string region_code;
		std::string client_version;
		std::string client_hash;
	};
}