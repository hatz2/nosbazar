#include "env.h"
#include "clientless.h"
#include <spdlog/spdlog.h>
#include "map_grid_repository.h"
#include <crow.h>

//int main(int argc, char** argv) {
//#ifdef _DEBUG
//	spdlog::set_level(spdlog::level::trace);
//#else
//	spdlog::set_level(spdlog::level::info);
//#endif
//
//	nosbazar::MapGridRepository::instance();
//
//	Env env;
//	nosbazar::Clientless client(env.account_id, env.world_server_id, env.world_server_channel);
//	return static_cast<int>(client.run());
//}

int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	nosbazar::MapGridRepository::instance();

	crow::SimpleApp app;

	CROW_ROUTE(app, "/search").methods("POST"_method)
	([](const crow::request& req) {
		SPDLOG_DEBUG(req.body);

		crow::json::rvalue json = crow::json::load(req.body);

		int server = json["server"].i();
		std::string packet = json["search_packet"].s();

		SPDLOG_DEBUG("Server: {}", server);
		SPDLOG_DEBUG("Packet: {}", packet);
		
		return crow::response(200, "good");
	});

	app.port(8080).multithreaded().run();
	
}

