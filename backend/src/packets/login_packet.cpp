#include "login_packet.h"
#include <fmt/format.h>
#include <random/random.h>
#include <sstream>
#include <strings/parse.h>
#include <charconv>
#include <strings/parse.h>

namespace nosbazar::packets::login {
	NoS0577Packet::NoS0577Packet(std::string session_token, std::string installation_id, std::string region_code, std::string client_version, std::string client_hash)
		: session_token(std::move(session_token))
		, installation_id(std::move(installation_id))
		, region_code(std::move(region_code))
		, client_version(std::move(client_version))
		, client_hash(std::move(client_hash))
	{
	}

	std::string NoS0577Packet::string()
	{
		return fmt::format("NoS0577 {}  {} {} {}\xB{} 0 {}", session_token, installation_id, random::random_hex_string(8), region_code, client_version, client_hash);
	}

	NsTeSTPacket::NsTeSTPacket(std::string_view packet)
	{
		

		std::string_view header = strings::token<std::string_view>(packet);
		strings::token<std::string_view>(packet); // unknown value
		strings::token<std::string_view>(packet); // blanks pace
		username = std::move(strings::token<std::string>(packet));

		// Skip first fields that have no interest for us
		for (int i = 0; i < 121; ++i) {
			strings::token<std::string_view>(packet);
		}

		session_id = strings::token<unsigned int>(packet);

		// Read world servers
		std::string world_server_data = strings::token<std::string>(packet);
		while (world_server_data != "-1:-1:-1:10000.10000.1") {
			servers.emplace_back(world_server_data);
			world_server_data = strings::token<std::string>(packet);
		};
	}

	std::optional<WorldServer> nosbazar::packets::login::NsTeSTPacket::find_world_server(int id, int channel)
	{
		for (const WorldServer& server : servers) {
			if (server.id == id && server.channel == channel) {
				return server;
			}
		}

		return std::nullopt;
	}

	WorldServer::WorldServer(const std::string& data)
	{
		std::string_view data_view = data;
		ip = strings::token<std::string_view>(data_view, ':');
		port = strings::token<uint16_t>(data_view, ':');
		capacity = strings::token<int>(data_view, ':');
		id = strings::token<int>(data_view, '.');
		channel = strings::token<int>(data_view, '.');
		name = strings::token<std::string_view>(data_view, '.');
	}
}


