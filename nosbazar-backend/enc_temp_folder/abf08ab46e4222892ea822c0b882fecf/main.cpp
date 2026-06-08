#include "net.h"
#include <spdlog/spdlog.h>
#include "auth/nosauth.h"
#include "dotenv.h"
#include <asio.hpp>
#include <random/random.h>
#include <login_packet.h>
#include <strings/hex.h>
#include <nosclient.h>
#include "env.h"
#include <auth/token_repository.h>
#include <algorithm>
#include <time/interval_timer.h>
#include "clientless.h"

namespace Server {
	static constexpr int dragonveil = 1;
	static constexpr int undercity = 12;
	static constexpr int nosfire = 13;
};

namespace {
	int target_server_id;
	int target_server_channel;
	uint16_t session_id;

	std::string world_server_ip;
	uint16_t world_server_port;
	uint16_t packet_counter;
	std::string account_name;

	int first_character_index = -1;
}

void handle_nstest(const std::string packet) {
	SPDLOG_DEBUG(packet);
	
	nosbazar::packets::login::NsTeSTPacket nstest(packet);

	session_id = nstest.session_id;
	account_name = nstest.username;

	auto server = nstest.find_world_server(static_cast<int>(target_server_id), target_server_channel);

	if (server) {
		world_server_ip = server->ip;
		world_server_port = server->port;
	}
}

int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	Env env;
	nosbazar::Clientless client(env.account_id, env.world_server_id, env.world_server_channel);
	client.run();
}


int old_main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	nosbazar::nosclient::check_and_download_outdated_files();

	Env env;

	target_server_id = env.world_server_id;
	target_server_channel = env.world_server_channel;

	auto identity = std::make_shared<nosbazar::auth::Identity>(env.identity_path);
	nosbazar::auth::NosAuth nosauth(identity);

	// Check if we already have a login token
	auto& token_repo = nosbazar::auth::TokenRepository::instance();
	auto login_token = token_repo.get_token(env.gf_email);

	if (login_token) {
		nosauth.set_login_token(login_token.value());
	}
	else {
		auto result = nosauth.authenticate({ .email = env.gf_email, .password = env.gf_password });

		if (result == nosbazar::auth::NosAuth::AuthResult::captcha) {
			// TODO: Solve captcha automatically
		}

		if (result != nosbazar::auth::NosAuth::AuthResult::ok) {
			SPDLOG_DEBUG("Auth failed");
			return EXIT_FAILURE;
		}
	}

	auto accs = nosauth.get_accounts();

	std::string account_id = env.account_id;

	std::optional<std::string> token;

	if (accs.contains(account_id)) {
		token = nosauth.get_session_token(account_id);
	}
	else {
		SPDLOG_ERROR("Could not find account");
	}

	asio::io_context context;

	auto client = std::make_unique<nosbazar::net::TCPClient>(context);
	client->connect(env.login_server_ip, env.login_server_port);

	nosbazar::packets::Publisher login_publisher;

	nosbazar::net::LoginSession login_session(std::move(client), login_publisher);
	login_publisher.subscribe("NsTeST", handle_nstest);

	std::string hex_token = nosbazar::strings::hexlify(token.value_or(""));
	std::string client_version = nosbazar::nosclient::get_nostale_client_version();
	std::string md5 = nosbazar::nosclient::get_md5_for_login_packet();

	nosbazar::packets::login::NoS0577Packet packet(hex_token, "NONE_CII", "0", client_version, md5);
	std::string login_packet = packet.string();
	login_session.send(login_packet);

	context.run();


	SPDLOG_DEBUG("World server -> {}:{}", world_server_ip, world_server_port);

	asio::io_context world_context;
	nosbazar::packets::Publisher world_publisher;
	
	// Make a new connection to the world server
	auto world_client = std::make_unique<nosbazar::net::TCPClient>(world_context);
	world_client->connect(world_server_ip, world_server_port);
	nosbazar::net::WorldSession world_session(std::move(world_client), world_publisher, session_id);

	// Subscribe to packets
	world_publisher.subscribe("clist", [](const std::string& packet) {
		SPDLOG_DEBUG(packet);

		std::istringstream iss(packet);

		std::string data;
		iss >> data; // header

		int char_index;
		iss >> char_index;

		first_character_index = std::min<int>(0, char_index);
	});

	world_publisher.subscribe("clist_end", [&world_session](const std::string& packet) {
		// Select character
		if (first_character_index >= 0) {
			world_session.send(fmt::format("select {}", first_character_index));
			world_session.send("game_start");
			world_session.send("lbs 0");
		}
	});

	constexpr int pulse_interval_secs = 60;

	auto send_pulse_cb = [&world_session]() {
		static uint64_t time = 0;
		time += pulse_interval_secs;

		std::string packet = fmt::format("pulse {} 0", time);
		SPDLOG_DEBUG(packet);
		world_session.send(packet);
		};

	nosbazar::time::IntervalTimer pulse_timer(
		world_context,
		std::chrono::seconds(pulse_interval_secs),
		send_pulse_cb
	);

	world_publisher.subscribe("OK", [&pulse_timer](const std::string& packet) {
		pulse_timer.start();
	});



	// Send the required packets first
	world_session.send(fmt::format("{}", session_id));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	world_session.send(fmt::format("{} GF 0", account_name));
	world_session.send("thisisgfmode");

	world_context.run();


	// TODO: Add GameClient class to encapsulate all this main function stuff
}

