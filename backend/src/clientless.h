#pragma once

#include <string>
#include "auth/nosauth.h"
#include <asio.hpp>
#include "net.h"
#include "env.h"
#include "packets/packet_publisher.h"
#include "time/interval_timer.h"
#include <game/sensors.h>
#include <agent/agent.h>

namespace nosbazar {
	class Clientless {
    public:
        enum class ExitCode : int {
            ok = 0,
            auth_failed = 1,
            login_failed = 2,
            world_failed = 3,
            no_character = 4,
            captcha = 5,
        };

        explicit Clientless(std::string account_id, std::shared_ptr<auth::NosAuth> nosauth);

        ~Clientless();

        ExitCode run();
        void stop();

    private:
        bool phase_authenticate();
        bool phase_login();
        bool phase_world();
        void phase_game();

        void on_nstest(std::string_view packet);
        
        void on_clist(std::string_view packet);
        void on_clist_end(std::string_view packet);
        void on_ok(std::string_view packet);
        void on_infoi(std::string_view packet);
        void on_success(std::string_view packet);

        void create_character();

        // Data obtained during the login protocol
        struct LoginResult {
            std::string world_ip;
            uint16_t    world_port{};
            uint16_t    session_id{};
            std::string account_name;
            int         world_server_id{};
            std::string world_server_name;
        };

        Env env;
        packets::Publisher packet_publisher;
        std::string account_id;

        std::shared_ptr<auth::NosAuth> nosauth;
        std::string session_token;

        asio::io_context login_context;
        asio::io_context world_context;
        std::unique_ptr<net::LoginSession> login_session;
        std::shared_ptr<net::WorldSession> world_session;

        std::optional<LoginResult> login_result;

        int first_char_index{ -1 };

        void init_pulse_timer();
        uint64_t pulse_time{0};
        std::unique_ptr<nosbazar::time::IntervalTimer> pulse_timer;

        // TODO: Add domain specific classes
        std::unique_ptr<game::Sensors> sensors;

        std::function<void()> schedule_tick;
        std::unique_ptr<asio::steady_timer> agent_timer;
        std::unique_ptr<agent::Agent> agent;

	};
}