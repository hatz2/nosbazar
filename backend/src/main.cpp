#include "env.h"
#include "clientless.h"
#include "auth/token_repository.h"
#include "nosclient.h"
#include <spdlog/spdlog.h>
#include "map_grid_repository.h"
#include <crow.h>
#include <bazar_search_queue.h>
#include <packets/c_blist.h>
#include <io/item_dat_parser.h>
#include <io/bcard_parser.h>
#include <io/nos_file_text_reader.h>
#include <crow/middlewares/cors.h>
#include <io/nsip_data_reader.h>
#include <thread>
#include <nlohmann/json.hpp>

int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	nosbazar::io::NosFileTextReader reader("C:/Program Files (x86)/Nostale/NostaleData/NSgtdData.NOS");
	nosbazar::io::ItemDatParser::instance().parse(reader.get_file_content("Item.dat"));
	nosbazar::io::BCardParser::instance().parse(reader.get_file_content("BCard.dat"));
	nosbazar::io::NSipDataReader::instance().initialize("C:/Program Files (x86)/Nostale/NostaleData/NSipData.NOS");

	nosbazar::MapGridRepository::instance();

	nosbazar::nosclient::check_and_download_outdated_files();

	Env env;
	auto identity = std::make_shared<nosbazar::auth::Identity>(env.identity_path);
	auto auth = std::make_shared<nosbazar::auth::NosAuth>(identity);

	auto& token_repo = nosbazar::auth::TokenRepository::instance();
	auto cached = token_repo.get_token(env.gf_email);

	if (cached) {
		auth->set_login_token(cached.value());
		SPDLOG_INFO("Reusing cached token for {}", env.gf_email);
	} else {
		auto auth_result = auth->authenticate({env.gf_email, env.gf_password});
		if (auth_result != nosbazar::auth::NosAuth::AuthResult::ok) {
			if (auth_result == nosbazar::auth::NosAuth::AuthResult::captcha) {
				SPDLOG_ERROR("Captcha required - not implemented yet");
			} else {
				SPDLOG_ERROR("Authentication failed");
			}
			return 1;
		}
	}

	auto accounts = auth->get_accounts();
	for (auto& entry : accounts.items()) {
		const std::string& id = entry.key();
		std::thread([id, auth]() {
			nosbazar::Clientless client(id, auth);
			return static_cast<int>(client.run());
		}).detach();
	}

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

	CROW_ROUTE(app, "/icon/<uint>").methods("GET"_method)
	([](const crow::request& req, uint32_t icon_id) {
		const std::vector<uint8_t>* data = nosbazar::io::NSipDataReader::instance().get_icon_data(icon_id);

		crow::response response;
		response.set_header("Content-Type", "image/png");

		if (data) {
			response.body.assign(reinterpret_cast<const char*>(data->data()), data->size());
		}
		else {
			response.code = 404;
			response.body = "Icon not found";
		}

		return response;
	});

	app.port(8080).multithreaded().run();

}