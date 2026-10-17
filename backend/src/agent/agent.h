#pragma once

#include <game/sensors.h>
#include <net.h>
#include "state_machine.h"
#include <packets/packet_publisher.h>
#include <bazar_search_queue.h>
#include <memory>

namespace nosbazar::agent {
	class SearchBazarState;

	class Agent {
	public:
		explicit Agent(net::WorldSession& session, packets::Publisher& publisher, GameServer server);

		void run(const game::Sensors& sensors);
		bool is_bazar_ready(const game::Sensors& sensors) const;

	private:
		std::shared_ptr<SearchBazarState> search_state;
		std::unique_ptr<StateMachine> state_machine;
	};
}
