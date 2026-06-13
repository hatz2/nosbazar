#pragma once

#include "state_machine.h"
#include <net.h>
#include <time/elapsed_timer.h>

namespace nosbazar::agent {
	class OpenBazarState : public State {
	public:
		explicit OpenBazarState(net::WorldSession& session);

		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;

	private:
		net::WorldSession& session;
		time::ElapsedTimer timer;
		int npc_id{};
	};
}