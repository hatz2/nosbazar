#pragma once

#pragma once

#include "state_machine.h"
#include <map_grid_repository.h>
#include <chrono>
#include <net.h>

namespace nosbazar::agent {
	class BazarPortalWalkState : public State {
	public:
		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;
	};

	class BazarNpcWalkState : public State {
	public:
		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;

	private:
		void generate_random_pos_around_npc(const game::Sensors& sensors);

		static constexpr int min_dist = 1;
		bool generated_pos{ false };
		int move_x{};
		int move_y{};
	};

	class BazarPortalEnterState : public State {
	public:
		explicit BazarPortalEnterState(net::WorldSession& session);
		bool should_execute(const game::Sensors& sensors) override;
		void act(const game::Sensors& sensors) override;

	private:
		std::chrono::steady_clock::time_point last_try;
		net::WorldSession& session;
	};
}