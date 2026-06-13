#include "walk_think_state.h"
#include <game/constants.h>
#include <random/random.h>

bool nosbazar::agent::BazarPortalWalkState::should_execute(const game::Sensors& sensors)
{
	if (!sensors.self_player->is_loaded()) {
		return false;
	}

	int map_id = sensors.self_player->map_id;

	if (map_id != game::mapids::nosville_map_id) {
		return false;
	}

	auto portal = sensors.scene->find_bazar_portal();

	if (!portal) {
		return false;
	}

	// true if dest pos doesn't match portal pos
	return sensors.self_player->dest_x != portal->x || sensors.self_player->dest_y != portal->y;
}

void nosbazar::agent::BazarPortalWalkState::act(const game::Sensors& sensors)
{
	auto portal = sensors.scene->find_bazar_portal();

	if (!portal) {
		return;
	}

	sensors.self_player->dest_x = portal->x;
	sensors.self_player->dest_y = portal->y;
}

bool nosbazar::agent::BazarNpcWalkState::should_execute(const game::Sensors& sensors)
{
	if (!sensors.self_player->is_loaded()) {
		return false;
	}

	int map_id = sensors.self_player->map_id;

	if (map_id != game::mapids::bazar_map_id) {
		return false;
	}

	auto npc = sensors.scene->find_bazar_npc();

	if (!npc) {
		return false;
	}

	if (!generated_pos) {
		return true;
	}
	else if (move_x != sensors.self_player->dest_x || move_y != sensors.self_player->dest_y) {
		return true;
	}

	return false;
}

void nosbazar::agent::BazarNpcWalkState::act(const game::Sensors& sensors)
{
	if (!generated_pos) {
		generate_random_pos_around_npc(sensors);
	}

	if (generated_pos) {
		sensors.self_player->dest_x = move_x;
		sensors.self_player->dest_y = move_y;
	}
}

void nosbazar::agent::BazarNpcWalkState::generate_random_pos_around_npc(const game::Sensors& sensors)
{
	auto npc = sensors.scene->find_bazar_npc();

	if (npc) {
		move_x = random::random_int(npc->x - min_dist, npc->x + min_dist);
		move_y = random::random_int(npc->y - min_dist, npc->y + min_dist);
		generated_pos = true;
	}
}

nosbazar::agent::BazarPortalEnterState::BazarPortalEnterState(net::WorldSession& session)
	: last_try(std::chrono::steady_clock::now())
	, session(session)
{
}

bool nosbazar::agent::BazarPortalEnterState::should_execute(const game::Sensors& sensors)
{
	auto portal = sensors.scene->find_bazar_portal();

	if (portal) {
		return sensors.self_player->x == portal->x && sensors.self_player->y == portal->y;
	}

	return false;
}

void nosbazar::agent::BazarPortalEnterState::act(const game::Sensors& sensors)
{
	constexpr int execute_time_ms = 1000;
	auto now = std::chrono::steady_clock::now();
	auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_try).count();

	if (time_diff > execute_time_ms) {
		session.send("preq");
		last_try = now;
	}
}
