#include "env.h"
#include "clientless.h"
#include "app_state.h"
#include "auth/token_repository.h"
#include "nosclient.h"
#include <spdlog/spdlog.h>
#include "map_grid_repository.h"
#include <crow.h>
#include <io/item_dat_parser.h>
#include <io/bcard_parser.h>
#include <io/monster_dat_parser.h>
#include <io/skill_dat_parser.h>
#include <io/nos_file_text_reader.h>
#include <crow/middlewares/cors.h>
#include <io/nsip_data_reader.h>
#include <io/const_string_parser.h>
#include <thread>
#include <vector>
#include <memory>
#include <filesystem>
#include <cctype>
#include <algorithm>
#include <string>
#include "api/routes.h"

namespace {

void setup_log_level()
{
    spdlog::level::level_enum log_level = spdlog::level::info;

	const char* level = std::getenv("LOG_LEVEL");
	if (level == nullptr) {
        log_level = spdlog::level::info;
	}
    else {
        std::string value(level);
        std::transform(value.begin(), value.end(), value.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        log_level = spdlog::level::from_str(value);
        if (log_level == spdlog::level::off && value != "off") {
            SPDLOG_WARN("Unknown LOG_LEVEL '{}', defaulting to info", value);
            log_level = spdlog::level::info;
        }
    }

    SPDLOG_INFO("Log level set to {}", spdlog::level::to_string_view(log_level));
    spdlog::set_level(log_level);
}

void initialize_game_data()
{
    nosbazar::nosclient::check_and_download_outdated_files();

    nosbazar::io::NosFileTextReader reader("assets/NostaleData/NSgtdData.NOS");
    SPDLOG_INFO("Parsing Item.dat...");
    nosbazar::io::ItemDatParser::instance().parse(reader.get_file_content("Item.dat"));
    SPDLOG_INFO("Parsing BCard.dat...");
    nosbazar::io::BCardParser::instance().parse(reader.get_file_content("BCard.dat"));
    SPDLOG_INFO("Parsing monster.dat...");
    nosbazar::io::MonsterDatParser::instance().parse(reader.get_file_content("monster.dat"));
    SPDLOG_INFO("Parsing Skill.dat...");
    nosbazar::io::SkillDatParser::instance().parse(reader.get_file_content("Skill.dat"));
    SPDLOG_INFO("Parsing NSipData.NOS...");
    nosbazar::io::NSipDataReader::instance().initialize("assets/NostaleData/NSipData.NOS");
    nosbazar::io::LangManager::get_instance();
    SPDLOG_INFO("Loading NScliData const strings...");
    nosbazar::io::ConstStringParser::instance();
    nosbazar::MapGridRepository::instance();
}

bool authenticate_and_spawn_clients(Env& env, std::vector<std::shared_ptr<nosbazar::Clientless>>& clients, std::vector<std::thread>& threads)
{
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
                SPDLOG_ERROR("Authentication failed. Auth result {}", fmt::underlying(auth_result));
            }
            return false;
        }
    }

    auto accounts = auth->get_accounts();
    uint16_t count = 0;
    for (auto& entry : accounts.items()) {
        if (count >= env.max_clients) {
            SPDLOG_WARN("MAX_CLIENTS ({}) reached, skipping remaining accounts", env.max_clients);
            break;
        }
        const std::string& id = entry.key();
        auto client = std::make_shared<nosbazar::Clientless>(id, auth);
        clients.push_back(client);
        threads.emplace_back([client]() {
            return static_cast<int>(client->run());
        });
        ++count;
    }

    return true;
}

} // anonymous namespace

int main(int argc, char** argv) {
	Env env;
	setup_log_level();

	initialize_game_data();

	std::vector<std::shared_ptr<nosbazar::Clientless>> clients;
	std::vector<std::thread> threads;
	if (!authenticate_and_spawn_clients(env, clients, threads)) {
        return EXIT_FAILURE;
    }

    crow::App<crow::CORSHandler> app;

    CROW_ROUTE(app, "/api/search").methods("POST"_method)(nosbazar::api::handle_search);
    CROW_ROUTE(app, "/api/items/static/<uint>").methods("GET"_method)(nosbazar::api::handle_item_static);
    CROW_ROUTE(app, "/api/items/name-search").methods("GET"_method)(nosbazar::api::handle_item_name_search);
    CROW_ROUTE(app, "/api/monster/static/<uint>").methods("GET"_method)(nosbazar::api::handle_monster_static);
    CROW_ROUTE(app, "/api/skill/static/<uint>").methods("GET"_method)(nosbazar::api::handle_skill_static);
    CROW_ROUTE(app, "/api/icon/<uint>").methods("GET"_method)(nosbazar::api::handle_icon);
    CROW_ROUTE(app, "/api/servers").methods("GET"_method)(nosbazar::api::handle_servers);
    CROW_ROUTE(app, "/api/conststring/<uint>").methods("GET"_method)(nosbazar::api::handle_const_string);
    CROW_ROUTE(app, "/api/conststring/version").methods("GET"_method)(nosbazar::api::handle_const_string_version);
    CROW_ROUTE(app, "/api/bcard/string").methods("GET"_method)(nosbazar::api::handle_bcard_string);

    app.port(8080).multithreaded().run();

	nosbazar::running = false;
	for (auto& client : clients) {
		client->stop();
	}
	for (auto& thread : threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}

	return 0;
}