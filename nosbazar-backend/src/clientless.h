#pragma once

#include <string>
#include "auth/nosauth.h"
#include <asio.hpp>
#include "net.h"
#include "env.h"
#include "packets/packet_publisher.h"
#include "time/interval_timer.h"
#include <game/sensors.h>

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

        explicit Clientless(std::string_view account_name, int world_server_id, int world_server_channel);

        ~Clientless();

        ExitCode run();

    private:
        bool phase_authenticate();
        bool phase_login();
        bool phase_world();
        void phase_game();

        void on_nstest(const std::string& packet);
        
        void on_clist(const std::string& packet);
        void on_clist_end(const std::string& packet);
        void on_ok(const std::string& packet);

        // Data obtained during the login protocol
        struct LoginResult {
            std::string world_ip;
            uint16_t    world_port{};
            uint16_t    session_id{};
            std::string account_name;
        };

        Env env;
        packets::Publisher packet_publisher;
        std::string account_id;
        int world_server_id;
        int world_server_channel;

        std::shared_ptr<auth::Identity> identity;
        std::unique_ptr<auth::NosAuth> nosauth;
        std::string session_token;

        asio::io_context login_context;
        asio::io_context world_context;
        std::unique_ptr<net::LoginSession> login_session;
        std::unique_ptr<net::WorldSession> world_session;

        std::optional<LoginResult> login_result;

        int first_char_index{ -1 };

        void init_pulse_timer();
        std::unique_ptr<nosbazar::time::IntervalTimer> pulse_timer;

        // TODO: Add domain specific classes
        std::unique_ptr<game::Sensors> sensors;

	};
}