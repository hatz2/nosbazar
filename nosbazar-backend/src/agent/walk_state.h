#pragma once

#include "state_machine.h"
#include <map_grid_repository.h>
#include <net.h>
#include <time/elapsed_timer.h>

namespace nosbazar::agent {
	class WalkState : public State {
	public:
		explicit WalkState(net::WorldSession& session);

		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;

	private:
		void find_path(const game::Sensors& sensors);
		void walk(const game::Sensors& sensors);

		std::vector<nosbazar::MapGrid::Cell> path;
		time::ElapsedTimer elapsed_timer;
		net::WorldSession& session;
	};
}