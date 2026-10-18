#pragma once

#include "state_machine.h"
#include <net.h>
#include <time/elapsed_timer.h>
#include <packets/packet_publisher.h>
#include <bazar_search_queue.h>
#include <chrono>

namespace nosbazar::agent {
	class SearchBazarState : public State {
	public:
		explicit SearchBazarState(net::WorldSession& session, packets::Publisher& publisher, GameServer server);
		~SearchBazarState() override;
		SearchBazarState(const SearchBazarState&) = delete;
		SearchBazarState& operator=(const SearchBazarState&) = delete;

		bool needs_open() const;
		bool is_initialized() const;
		void begin_open();
		// Check deadlines even when another agent behavior has priority.
		bool poll();

		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;

	private:
		enum class Exchange { unopened, opening, ready, searching, failed };
		void on_rc_blist(std::string_view packet);
		void fail_exchange(BazarSearchError error);

		static constexpr int search_delay_ms = 3500;
		net::WorldSession& session;
		time::ElapsedTimer timer;
		std::shared_ptr<BazarSearch> task;
		GameServer server;
		Exchange exchange{Exchange::unopened};
		std::chrono::steady_clock::time_point opening_deadline{};
		packets::Publisher::Subscription subscription;

	};
}
