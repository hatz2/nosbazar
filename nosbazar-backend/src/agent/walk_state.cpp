#include "walk_state.h"
#include <spdlog/spdlog.h>
#include <packets/walk.h>

nosbazar::agent::WalkState::WalkState(net::WorldSession& session) 
	: session(session)
	, last_walk_time(std::chrono::steady_clock::now())
{
}

bool nosbazar::agent::WalkState::should_execute(const game::Sensors& sensors)
{
	if (sensors.self_player->wants_to_walk()) {
		return true;
	}

	if (!path.empty()) {
		path.clear();
	}

	return false;
}

void nosbazar::agent::WalkState::act(const game::Sensors& sensors)
{
	if (path.empty()) {
		find_path(sensors);
	}
	
	walk(sensors);
}

void nosbazar::agent::WalkState::find_path(const game::Sensors& sensors)
{
	int map_id = sensors.self_player->map_id;

	if (map_id) {
		auto map_grid = nosbazar::MapGridRepository::instance().grid(map_id);
		if (map_grid.has_value()) {
			

			nosbazar::MapGrid::Cell origin{ sensors.self_player->x, sensors.self_player->y };
			nosbazar::MapGrid::Cell dest{ sensors.self_player->dest_x, sensors.self_player->dest_y };
			this->path = map_grid->get().find_path(origin, dest);

			// remove first pos as it's the current location
			if (!path.empty()) {
				path.erase(path.begin()); 
			}
		}
	}
}

void nosbazar::agent::WalkState::walk(const game::Sensors& sensors)
{
	if (path.empty()) {
		return;
	}

	constexpr auto walk_time_ms = 750;
	auto now = std::chrono::steady_clock::now();
	auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_walk_time).count();

	if (time_diff > walk_time_ms) {
		last_walk_time = std::chrono::steady_clock::now();
		nosbazar::MapGrid::Cell cell;

		// Skip first STEP - 1 cells
		int step = sensors.self_player->walk_step();
		for (int i = 0; i < (step - 1) && !path.empty(); ++i) {
			cell = path.front();
			path.erase(path.begin());
		}

		if (cell.x && cell.y) {
			packets::Walk packet(
				cell.x,
				cell.y,
				0,
				sensors.self_player->speed
			);

			session.send(packet.string());

			if (!path.empty()) {
				path.erase(path.begin());
			}
		}
		else {
			SPDLOG_DEBUG("Cannot walk because x or y position is 0");
		}
	}
}
