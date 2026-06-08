#include "clientless.h"
#include "env.h"
#include "nosclient.h"
#include "auth/token_repository.h"
#include "packets/login_packet.h"
#include "strings/hex.h"
#include "strings/parse.h"
#include <algorithm>
#include <spdlog/spdlog.h>

nosbazar::Clientless::Clientless(std::string_view account_name, int world_server_id, int world_server_channel)
	: account_id(account_name)
	, world_server_id(world_server_id)
	, world_server_channel(world_server_channel)
	, identity(std::make_shared<auth::Identity>(env.identity_path))
	, nosauth(std::make_unique<auth::NosAuth>(identity))
{
    init_pulse_timer();
}

nosbazar::Clientless::~Clientless()
{
}

nosbazar::Clientless::ExitCode nosbazar::Clientless::run()
{
    nosclient::check_and_download_outdated_files();

    if (!phase_authenticate()) {
        return ExitCode::auth_failed;
    }

    if (!phase_login()) {
        return ExitCode::login_failed;
    }

    if (!phase_world()) {
        return ExitCode::world_failed;
    }

    phase_game();

    return ExitCode::ok;
}

bool nosbazar::Clientless::phase_authenticate()
{
    // Check if we already have a login token
    auto& token_repo = nosbazar::auth::TokenRepository::instance();
    std::optional<std::string> cached = token_repo.get_token(env.gf_email);

    if (cached) {
        nosauth->set_login_token(cached.value());
        SPDLOG_DEBUG("Reusing cached token for {}", env.gf_email);
    }
    else {
        auto result = nosauth->authenticate({
            .email = env.gf_email,
            .password = env.gf_password
        });

        if (result == auth::NosAuth::AuthResult::captcha) {
            SPDLOG_ERROR("Captcha is needed to solve - not implemented yet");
            return false;  // TODO: automatic captcha solver
        }

        if (result != auth::NosAuth::AuthResult::ok) {
            SPDLOG_ERROR("Failed authentication");
            return false;
        }
    }

    auto accounts = nosauth->get_accounts();
    if (!accounts.contains(account_id)) {
        SPDLOG_ERROR("Could not find account with id: {}", account_id);
        return false;
    }

    auto token = nosauth->get_session_token(account_id);
    if (!token) {
        SPDLOG_ERROR("Could not obtain session token");
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
        SPDLOG_ERROR(
            "NsTeST not received - server not found (id={}, ch={})", 
            env.world_server_id, 
            env.world_server_channel
        );
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

    world_session = std::make_unique<net::WorldSession>(std::move(client), packet_publisher, lr.session_id);

    // Subscribe to packets
    packet_publisher.subscribe("clist", [this](const std::string& p) { on_clist(p); });
    packet_publisher.subscribe("clist_end", [this](const std::string& p) { on_clist_end(p); });
    packet_publisher.subscribe("OK", [this](const std::string& p) { on_ok(p); });

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
    //sensors = std::make_unique<GameSensors>(bus);
    //game_state = std::make_unique<GameState>(bus);
    //agent = std::make_unique<BotAgent>(bus, *sensors, *game_state, *world_session);

    // Pulse packet keep alive
    pulse_timer->start();

    // Agent tick is integrated into io_context to avoid blocking the main thread
    auto agent_timer = std::make_shared<asio::steady_timer>(world_context);

    std::function<void()> schedule_tick = [&]() {
        agent_timer->expires_after(std::chrono::milliseconds(100));
        agent_timer->async_wait([&](const asio::error_code& ec) {
            if (!ec) {
                //agent->tick();
                schedule_tick();
            }
            });
        };

    schedule_tick();

    // world_context.run() is already running from phase_world()
    // no need to call it again
}

void nosbazar::Clientless::on_nstest(const std::string& packet)
{
    SPDLOG_DEBUG("NsTeST: {}", packet);

    packets::login::NsTeSTPacket nstest(packet);

    auto server = nstest.find_world_server(
        env.world_server_id,
        env.world_server_channel
    );

    if (!server) {
        SPDLOG_WARN(
            "Server id={} ch={} not found in NsTeST",
            env.world_server_id, 
            env.world_server_channel
        );
        return;
    }

    login_result = LoginResult{
        .world_ip = server->ip,
        .world_port = server->port,
        .session_id = nstest.session_id,
        .account_name = nstest.username,
    };
}

void nosbazar::Clientless::on_clist(const std::string& packet)
{
    SPDLOG_DEBUG("clist: {}", packet);

    std::string_view packet_view;
    std::string_view header = strings::token<std::string_view>(packet_view);
    int char_index = strings::token<int>(packet_view);

    // Save the lowest possible index (first slot character)
    if (first_char_index < 0) {
        first_char_index = char_index;
    }
    else {
        first_char_index = std::min(first_char_index, char_index);
    }
}

void nosbazar::Clientless::on_clist_end(const std::string& packet)
{
    if (first_char_index < 0) {
        SPDLOG_ERROR("No character was found in the account");

        // TODO: Create new character if no characters are in the account
        world_context.stop();
        return;
    }

    SPDLOG_INFO("Selecting character on slot {}", first_char_index);
    world_session->send(fmt::format("select {}", first_char_index));
    world_session->send("game_start");
    world_session->send("lbs 0");
}

void nosbazar::Clientless::on_ok(const std::string& packet)
{
    SPDLOG_INFO("Server ready - starting game phase");
    phase_game();
}

void nosbazar::Clientless::init_pulse_timer()
{
    constexpr int pulse_secs = 60;
    static uint64_t pulse_time = 0;

    pulse_timer = std::make_unique<nosbazar::time::IntervalTimer>(
        world_context,
        std::chrono::seconds(pulse_secs),
        [this]() {
            pulse_time += pulse_secs;
            world_session->send(fmt::format("pulse {} 0", pulse_time));
            SPDLOG_DEBUG("Sending pulse");
        }
    );
}
