#pragma once

#include "state_machine.h"
#include <net.h>
#include <time/elapsed_timer.h>
#include <packets/packet_publisher.h>
#include <bazar_search_queue.h>

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

		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;

	private:
		enum class Exchange { unopened, opening, ready, searching };
		void on_rc_blist(std::string_view packet);

		static constexpr int search_delay_ms = 3500;
		net::WorldSession& session;
		time::ElapsedTimer timer;
		std::shared_ptr<BazarSearch> task;
		GameServer server;
		Exchange exchange{Exchange::unopened};
		packets::Publisher::Subscription subscription;

	};
}
