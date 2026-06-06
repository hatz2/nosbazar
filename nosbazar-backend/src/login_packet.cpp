#include "login_packet.h"
#include <fmt/format.h>
#include <random/random.h>
#include <sstream>
#include <strings/parse.h>
#include <charconv>

nosbazar::packets::login::NoS0577Packet::NoS0577Packet(std::string session_token, std::string installation_id, std::string region_code, std::string client_version, std::string client_hash)
	: session_token(std::move(session_token))
	, installation_id(std::move(installation_id))
	, region_code(std::move(region_code))
	, client_version(std::move(client_version))
	, client_hash(std::move(client_hash))
{
}

std::string nosbazar::packets::login::NoS0577Packet::string()
{
	return fmt::format("NoS0577 {}  {} {} {}\xB{} 0 {}", session_token, installation_id, random::random_hex_string(8), region_code, client_version, client_hash);
}

nosbazar::packets::login::NsTeSTPacket::NsTeSTPacket(const std::string& packet)
{
	std::string dummy;
	
	std::istringstream stream(packet);

	// Skip first fields that have no interest for us
	for (int i = 0; i < 124; ++i) {
		stream >> dummy;
	}

	stream >> session_id;

	// Read world servers
	std::string world_server_data;
	while (stream >> world_server_data) {
		servers.emplace_back(world_server_data);
	}
}

nosbazar::packets::login::NsTeSTPacket::WorldServer::WorldServer(const std::string& data)
{
	std::string_view data_view = data;
	ip = strings::token<std::string_view>(data_view, ':');
	port = strings::token<uint16_t>(data_view, ':');
	capacity = strings::token<int>(data_view, ':');
	id = strings::token<int>(data_view, '.');
	channel = strings::token<int>(data_view, '.');
	name = strings::token<std::string_view>(data_view, '.');
}
