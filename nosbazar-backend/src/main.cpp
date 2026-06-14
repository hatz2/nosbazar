#include "env.h"
#include "clientless.h"
#include <spdlog/spdlog.h>
#include "map_grid_repository.h"
#include <crow.h>
#include <bazar_search_queue.h>

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

	std::thread([&]() {
		Env env;
		nosbazar::Clientless client(env.account_id, env.world_server_id, env.world_server_channel);
		return static_cast<int>(client.run());
	}).detach();

	crow::SimpleApp app;

	CROW_ROUTE(app, "/search").methods("POST"_method)
	([](const crow::request& req) {
		SPDLOG_DEBUG(req.body);

		crow::json::rvalue json = crow::json::load(req.body);

		int server = json["server"].i();
		std::string packet = json["search_packet"].s();

		auto task = std::make_shared<nosbazar::BazarSearch>();
		task->request.packet = packet;

		nosbazar::BazarSearchManager::queue(server).push(task);

		SPDLOG_DEBUG("Server: {}", server);
		SPDLOG_DEBUG("Packet: {}", packet);

		std::string result = task->response.packet.get_future().get();

		
		return crow::response(200, result);
	});

	app.port(8080).multithreaded().run();
	
}

