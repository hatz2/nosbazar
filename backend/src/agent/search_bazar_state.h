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

		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;

	private:
		void on_rc_blist(std::string_view packet);

		static constexpr int search_delay_ms = 3500;
		net::WorldSession& session;
		time::ElapsedTimer timer;
		std::shared_ptr<BazarSearch> task;
		GameServer server;

	};
}