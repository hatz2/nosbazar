#pragma once

#include "state_machine.h"
#include <net.h>
#include <time/elapsed_timer.h>

namespace nosbazar::agent {
	class SearchBazarState;

	class OpenBazarState : public State {
	public:
		OpenBazarState(net::WorldSession& session, SearchBazarState& search);

		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;

	private:
		net::WorldSession& session;
		SearchBazarState& search;
		time::ElapsedTimer timer;
		int npc_id{};
	};
}
