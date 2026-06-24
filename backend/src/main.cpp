#include "env.h"
#include "clientless.h"
#include <spdlog/spdlog.h>
#include "map_grid_repository.h"
#include <crow.h>
#include <bazar_search_queue.h>
#include <packets/c_blist.h>
#include <io/item_dat_parser.h>
#include <io/bcard_parser.h>
#include <io/nos_file_text_reader.h>
#include <crow/middlewares/cors.h>

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

//struct CORSMiddleware {
//	struct context {};
//
//	void before_handle(crow::request& req, crow::response& res, context&)
//	{
//		if (req.method == crow::HTTPMethod::Options)
//		{
//			res.add_header("Access-Control-Allow-Origin", "*");
//			res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
//			res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
//			res.code = 204;
//			res.end();
//			return;
//		}
//	}
//
//	void after_handle(crow::request&, crow::response& res, context&)
//	{
//		res.add_header("Access-Control-Allow-Origin", "*");
//		res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
//	}
//};

int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	nosbazar::io::NosFileTextReader reader("C:/Program Files (x86)/Nostale/NostaleData/NSgtdData.NOS");
	nosbazar::io::ItemDatParser::instance().parse(reader.get_file_content("Item.dat"));
	nosbazar::io::BCardParser::instance().parse(reader.get_file_content("BCard.dat"));

	nosbazar::MapGridRepository::instance();

	std::thread([&]() {
		Env env;
		nosbazar::Clientless client(env.account_id, env.world_server_id, env.world_server_channel);
		return static_cast<int>(client.run());
	}).detach();

	//crow::App<CORSMiddleware> app;
	crow::App<crow::CORSHandler> app;

	CROW_ROUTE(app, "/search").methods("POST"_method)
	([](const crow::request& req) {
		SPDLOG_DEBUG(req.body);

		crow::json::rvalue json = crow::json::load(req.body);

		int64_t server = json["server"].i();

		auto task = std::make_shared<nosbazar::BazarSearch>();

		nosbazar::packets::CBlist search_packet{
			.index = (int)json["filters"]["index"].i(),
			.type_filter = (int)json["filters"]["category"].i(),
			.sub_type_filter = (int)json["filters"]["sub_category"].i(),
			.level_filter = (int)json["filters"]["level"].i(),
			.rare_filter = (int)json["filters"]["rare"].i(),
			.upgrade_filter = (int)json["filters"]["upgrade"].i(),
			.order_filter = (int)json["filters"]["order"].i()
		};

		for (auto& vnum : json["filters"]["vnum"].lo()) {
			search_packet.vnums_filter.push_back(vnum.i());
		}

		task->request.search_packet = std::move(search_packet);

		nosbazar::BazarSearchManager::queue(server).push(task);

		std::string result = task->response.packet.get_future().get();

		return crow::response(result);
	});

	// GET /items/static/{vnum}: Returns full item details for a given vnum.
	CROW_ROUTE(app, "/items/static/<uint>").methods("GET"_method)
	([](const crow::request& req, uint32_t vnum) {
		try {
			// Get the item data using the singleton parser instance
			const nosbazar::io::Item& item = nosbazar::io::ItemDatParser::instance().item_data(vnum);

			// Call the new json() method and serialize to string for Crow response
			nlohmann::json json_data = item.json();
			std::string result = json_data.dump(2);

			return crow::response(result);
		} catch (const std::out_of_range& e) {
			SPDLOG_WARN("Item not found for vnum: {}", vnum);
			return crow::response(404, "Item data not found.");
		} catch (const std::exception& e) {
			SPDLOG_ERROR("Internal error fetching item data for vnum {} : {}", vnum, e.what());
			return crow::response(500, "Internal Server Error.");
		}
	});

	app.port(8080).multithreaded().run();

}


//#include <io/item_dat_parser.h>
//#include <io/nos_file_text_reader.h>
//#include <io/lang_file_parser.h>

//int main(int argc, char** argv) {
//    //nosbazar::io::NosFileTextReader reader("C:/Program Files (x86)/Nostale/NostaleData/NSgtdData.NOS");
//    //nosbazar::io::ItemDatParser parser(reader.get_file_content("Item.dat"));

//    nosbazar::io::NosFileTextReader reader("C:/Program Files (x86)/Nostale/NostaleData/NSlangData_ES.NOS");
//    nosbazar::io::LangFileParser parser(reader.get_file_content("_code_es_Item.txt"));
//}