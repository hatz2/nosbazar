#include "clientless.h"
#include "env.h"
#include "nosclient.h"
#include "app_state.h"
#include "packets/login_packet.h"
#include "world_server_assigner.h"
#include "server_registry.h"
#include "strings/hex.h"
#include "strings/parse.h"
#include <algorithm>
#include <spdlog/spdlog.h>
#include <random/random.h>

nosbazar::Clientless::Clientless(std::string account_id, std::shared_ptr<auth::NosAuth> nosauth)
	: account_id(std::move(account_id))
	, nosauth(std::move(nosauth))
    , sensors(std::make_unique<game::Sensors>(packet_publisher))
    , agent(nullptr)
{
    init_pulse_timer();
}

nosbazar::Clientless::~Clientless()
{
}

void nosbazar::Clientless::stop()
{
	login_context.stop();
	world_context.stop();
}

nosbazar::Clientless::ExitCode nosbazar::Clientless::run()
{
    if (!phase_authenticate()) {
        SPDLOG_ERROR("Auth phase failed");
        return ExitCode::auth_failed;
    }

    if (!phase_login()) {
        SPDLOG_ERROR("Login phase failed");
        return ExitCode::login_failed;
    }

    if (!phase_world()) {
        SPDLOG_ERROR("World phase failed");
        return ExitCode::world_failed;
    }

    phase_game();

    return ExitCode::ok;
}

bool nosbazar::Clientless::phase_authenticate()
{
    auto token = nosauth->get_session_token(account_id);
    if (!token) {
        SPDLOG_ERROR("Could not obtain session token for account {}", account_id);
        return false;
    }

    session_token = std::move(token.value());
    return true;
}

bool nosbazar::Clientless::phase_login()
{
    auto client = std::make_unique<net::TCPClient>(login_context);
    client->connect(env.login_server_ip, env.login_server_port);

    login_session = std::make_unique<net::LoginSession>(std::move(client), packet_publisher);
    packet_publisher.subscribe("NsTeST", [this](const std::string& p) { on_nstest(p); });

    std::string hex_token = strings::hexlify(session_token);
    std::string client_version = nosclient::get_nostale_client_version();
    std::string md5 = nosclient::get_md5_for_login_packet();

    packets::login::NoS0577Packet packet(hex_token, "NONE_CII", "0", client_version, md5);
    login_session->send(packet.string());

    login_context.run();  // block untill login session is closed by the remote

    if (!login_result) {
        SPDLOG_ERROR("NsTeST not received - no servers available");
        return false;
    }

    SPDLOG_INFO(
        "World server: {}:{}", 
        login_result->world_ip,
        login_result->world_port
    );

    return true;
}

bool nosbazar::Clientless::phase_world()
{
    LoginResult& lr = login_result.value();

    auto client = std::make_unique<net::TCPClient>(world_context);
    client->connect(lr.world_ip, lr.world_port);

    world_session = std::make_shared<net::WorldSession>(std::move(client), packet_publisher, lr.session_id);

    // Subscribe to packets
    packet_publisher.subscribe("clist", [this](auto& packet) { on_clist(packet); });
    packet_publisher.subscribe("clist_end", [this](auto& packet) { on_clist_end(packet); });
    packet_publisher.subscribe("OK", [this](auto& packet) { on_ok(packet); });
    packet_publisher.subscribe("infoi", [this](auto& packet) { on_infoi(packet); });
    packet_publisher.subscribe("success", [this](auto& packet) { on_success(packet); });

    // Initial packets sent to the server
    world_session->send(fmt::format("{}", lr.session_id));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    world_session->send(fmt::format("{} GF 0", lr.account_name));
    world_session->send("thisisgfmode");

    world_context.run();  // block untill we get kicked out of the world server

    return true;
}

void nosbazar::Clientless::phase_game()
{
    // Make domain instances
    //game_state = std::make_unique<GameState>(bus);
    ServerRegistry::instance().register_server(login_result->world_server_id, login_result->world_server_name);

    agent = std::make_unique<agent::Agent>(*world_session, packet_publisher, login_result->world_server_id);

    // Pulse packet keep alive
    pulse_timer->start();

    // Agent tick is integrated into io_context to avoid blocking the main thread
    agent_timer = std::make_unique<asio::steady_timer>(world_context);

    schedule_tick = [&]() {
        agent_timer->expires_after(std::chrono::milliseconds(10));
        agent_timer->async_wait([&](const asio::error_code& ec) {
            if (!ec && !running) {
                return;
            }
            if (!ec) {
                agent->run(*sensors);
                schedule_tick();
            }
            else if (running) {
                SPDLOG_ERROR(ec.message());
            }
        });
    };

    schedule_tick();

    // world_context.run() is already running from phase_world()
    // no need to call it again
}

void nosbazar::Clientless::on_nstest(std::string_view packet)
{
    SPDLOG_DEBUG("NsTeST: {}", packet);

    packets::login::NsTeSTPacket nstest(packet);

    if (nstest.servers.empty()) {
        SPDLOG_WARN("No servers available in NsTeST");
        return;
    }

    auto& server = WorldServerAssigner::instance().assign(nstest.servers);

    login_result = LoginResult{
        .world_ip = server.ip,
        .world_port = server.port,
        .session_id = nstest.session_id,
        .account_name = nstest.username,
        .world_server_id = server.id,
        .world_server_name = server.name,
    };
}

void nosbazar::Clientless::on_clist(std::string_view packet)
{
    SPDLOG_DEBUG("clist: {}", packet);

    std::string_view header = strings::token<std::string_view>(packet);
    int char_index = strings::token<int>(packet);

    // Save the lowest possible index (first slot character)
    if (first_char_index < 0) {
        first_char_index = char_index;
    }
    else {
        first_char_index = std::min(first_char_index, char_index);
    }
}

void nosbazar::Clientless::on_clist_end(std::string_view packet)
{
    if (first_char_index < 0) {
        SPDLOG_DEBUG("No character was found in the account");
        create_character();
        return;
    }

    SPDLOG_INFO("Selecting character on slot {}", first_char_index);
    world_session->send(fmt::format("select {}", first_char_index));
    world_session->send("game_start");
    world_session->send("lbs 0");
}

void nosbazar::Clientless::on_ok(std::string_view packet)
{
    SPDLOG_INFO("Server ready - starting game phase");
    phase_game();
}

void nosbazar::Clientless::on_infoi(std::string_view packet)
{
    std::string_view character_name_already_in_use = "infoi 875 0 0 0";

    if (packet == character_name_already_in_use) {
        SPDLOG_DEBUG("Character name is already in use");
        create_character();
    }
}

void nosbazar::Clientless::on_success(std::string_view packet)
{
    SPDLOG_DEBUG("Character created successfully");
}

void nosbazar::Clientless::create_character()
{
    constexpr size_t name_length = 14;
    std::string name = random::random_character_name(name_length);
    world_session->send(fmt::format("Char_NEW {} 0 1 0 9", name));

    SPDLOG_DEBUG("Creating character {}", name);
}

void nosbazar::Clientless::init_pulse_timer()
{
    constexpr int pulse_secs = 60;

    pulse_timer = std::make_unique<nosbazar::time::IntervalTimer>(
        world_context,
        std::chrono::seconds(pulse_secs),
        [this]() {
            pulse_time += pulse_secs;
            world_session->send(fmt::format("pulse {} 0", pulse_time));
        }
    );
}
