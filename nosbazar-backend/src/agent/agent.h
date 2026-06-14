#pragma once

#include <game/sensors.h>
#include <net.h>
#include "state_machine.h"
#include <packets/packet_publisher.h>
#include <bazar_search_queue.h>

namespace nosbazar::agent {
	class Agent {
	public:
		explicit Agent(net::WorldSession& session, packets::Publisher& publisher, GameServer server);

		void run(const game::Sensors& sensors);

	private:
		std::unique_ptr<StateMachine> state_machine;
	};
}