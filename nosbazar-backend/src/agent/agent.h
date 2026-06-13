#pragma once

#include <game/sensors.h>
#include <net.h>
#include "state_machine.h"

namespace nosbazar::agent {
	class Agent {
	public:
		explicit Agent(net::WorldSession& session);

		void run(const game::Sensors& sensors);

	private:
		std::unique_ptr<StateMachine> state_machine;
	};
}