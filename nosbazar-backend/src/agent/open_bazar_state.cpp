#include "open_bazar_state.h"
#include <distances.h>
#include <fmt/format.h>

nosbazar::agent::OpenBazarState::OpenBazarState(net::WorldSession& session) : session(session)
{
}

bool nosbazar::agent::OpenBazarState::should_execute(const game::Sensors& sensors)
{
	if (sensors.is_bazar_opened) {
		return false;
	}

	constexpr double distance_to_npc = 2.5;
	auto bazar_npc = sensors.scene->find_bazar_npc();

	if (bazar_npc) {
		npc_id = bazar_npc->id;
		double dist = distances::chebyshev(bazar_npc->x, bazar_npc->y, sensors.self_player->x, sensors.self_player->y);
		return dist <= distance_to_npc;
	}

	return false;
}

void nosbazar::agent::OpenBazarState::act(const game::Sensors& sensors)
{
	constexpr int64_t execution_time_ms = 2000;
	if (timer.elapsed() < execution_time_ms) {
		return;
	}

	session.send(fmt::format("npc_req 2 {}", npc_id));
	session.send(fmt::format("n_run 60 0 2 {}", npc_id));
	session.send("c_blist  0 0 0 0 0 0 0 0 0");

	timer.start();
}
