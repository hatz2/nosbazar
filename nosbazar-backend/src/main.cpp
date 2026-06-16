#include "env.h"
#include "clientless.h"
#include <spdlog/spdlog.h>
#include "map_grid_repository.h"
#include <crow.h>
#include <bazar_search_queue.h>
#include <packets/c_blist.h>
#include <io/nsgtd_data_reader.h>

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


//int main(int argc, char** argv) {
//#ifdef _DEBUG
//	spdlog::set_level(spdlog::level::trace);
//#else
//	spdlog::set_level(spdlog::level::info);
//#endif
//
//	nosbazar::MapGridRepository::instance();
//
//	std::thread([&]() {
//		Env env;
//		nosbazar::Clientless client(env.account_id, env.world_server_id, env.world_server_channel);
//		return static_cast<int>(client.run());
//	}).detach();
//
//	crow::SimpleApp app;
//
//	CROW_ROUTE(app, "/search").methods("POST"_method)
//	([](const crow::request& req) {
//		SPDLOG_DEBUG(req.body);
//
//		crow::json::rvalue json = crow::json::load(req.body);
//
//		int server = json["server"].i();
//		//std::string packet = json["search_packet"].s();
//
//		auto task = std::make_shared<nosbazar::BazarSearch>();
//		//task->request.packet = packet;
//
//		nosbazar::packets::CBlist search_packet{
//			.index = (int)json["filters"]["index"].i(),
//			.type_filter = (int)json["filters"]["category"].i(),
//			.sub_type_filter = (int)json["filters"]["sub_category"].i(),
//			.level_filter = (int)json["filters"]["level"].i(),
//			.rare_filter = (int)json["filters"]["rare"].i(),
//			.upgrade_filter = (int)json["filters"]["upgrade"].i(),
//			.order_filter = (int)json["filters"]["order"].i()
//		};
//
//		for (auto& vnum : json["filters"]["vnum"].lo()) {
//			search_packet.vnums_filter.push_back(vnum.i());
//		}
//
//		nosbazar::BazarSearchManager::queue(server).push(task);
//
//		std::string result = task->response.packet.get_future().get();
//
//		
//		return crow::response(result);
//	});
//
//	app.port(8080).multithreaded().run();
//	
//}

#include <io/item_dat_parser.h>

int main(int argc, char** argv) {
    nosbazar::io::NSgtdDataReader reader("C:/Program Files (x86)/Nostale/NostaleData/NSgtdData.NOS");
    nosbazar::io::ItemDatParser parser(reader.get_item_dat_file());
}

