#include "login_packet.h"
#include <fmt/format.h>
#include <random/random.h>

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
