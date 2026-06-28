#include "env.h"
#include "clientless.h"
#include "auth/token_repository.h"
#include "nosclient.h"
#include <spdlog/spdlog.h>
#include "map_grid_repository.h"
#include <crow.h>
#include <io/item_dat_parser.h>
#include <io/bcard_parser.h>
#include <io/nos_file_text_reader.h>
#include <crow/middlewares/cors.h>
#include <io/nsip_data_reader.h>
#include <io/const_string_parser.h>
#include <thread>
#include "api/routes.h"

namespace {

void setup_log_level()
{
#ifdef _DEBUG
    spdlog::set_level(spdlog::level::trace);
#else
    spdlog::set_level(spdlog::level::info);
#endif
}

void initialize_game_data()
{
    nosbazar::nosclient::check_and_download_outdated_files();

    nosbazar::io::NosFileTextReader reader("assets/NostaleData/NSgtdData.NOS");
    SPDLOG_INFO("Parsing Item.dat...");
    nosbazar::io::ItemDatParser::instance().parse(reader.get_file_content("Item.dat"));
    SPDLOG_INFO("Parsing BCard.dat...");
    nosbazar::io::BCardParser::instance().parse(reader.get_file_content("BCard.dat"));
    SPDLOG_INFO("Parsing NSipData.NOS...");
    nosbazar::io::NSipDataReader::instance().initialize("assets/NostaleData/NSipData.NOS");
    nosbazar::io::LangManager::get_instance();
    SPDLOG_INFO("Loading NScliData const strings...");
    nosbazar::io::ConstStringParser::instance();
    nosbazar::MapGridRepository::instance();
}

bool authenticate_and_spawn_clients(Env& env)
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
                SPDLOG_ERROR("Authentication failed");
            }
            return false;
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

    return true;
}

} // anonymous namespace

int main(int argc, char** argv) {
    setup_log_level();

    initialize_game_data();

    Env env;
    if (!authenticate_and_spawn_clients(env)) {
        return EXIT_FAILURE;
    }

    crow::App<crow::CORSHandler> app;

    CROW_ROUTE(app, "/search").methods("POST"_method)(nosbazar::api::handle_search);
    CROW_ROUTE(app, "/items/static/<uint>").methods("GET"_method)(nosbazar::api::handle_item_static);
    CROW_ROUTE(app, "/icon/<uint>").methods("GET"_method)(nosbazar::api::handle_icon);
    CROW_ROUTE(app, "/servers").methods("GET"_method)(nosbazar::api::handle_servers);
    CROW_ROUTE(app, "/conststring/<uint>").methods("GET"_method)(nosbazar::api::handle_const_string);

    app.port(8080).multithreaded().run();
}