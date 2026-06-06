#pragma once

#include <string>
#include <vector>

namespace nosbazar::packets::login {
	class NoS0577Packet {
	public:
		explicit NoS0577Packet(std::string hex_session_token, std::string installation_id, std::string region_code, std::string client_version, std::string client_hash);

		std::string string();
	private:
		std::string session_token;
		std::string installation_id;
		std::string region_code;
		std::string client_version;
		std::string client_hash;
	};

	class NsTeSTPacket {
	public:
		explicit NsTeSTPacket(const std::string& packet);

		struct WorldServer {
			int id;
			int channel;
			int capacity;
			uint16_t port;
			std::string ip;
			std::string name;

			explicit WorldServer(const std::string& data);
			WorldServer() = delete;
		};

		std::vector<WorldServer> servers;
		uint16_t session_id = 0;
	};
}