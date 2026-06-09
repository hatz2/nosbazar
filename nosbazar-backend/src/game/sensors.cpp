#include "sensors.h"

#include <packets/in.h>
#include <packets/c_map.h>
#include <packets/gp_packet.h>
#include <packets/at.h>
#include <packets/cond.h>

#include <spdlog/spdlog.h>
#include "constants.h"

namespace nosbazar::game {
	SelfPlayer::SelfPlayer(packets::Publisher& publisher)
	{
		publisher.subscribe(packets::At::opcode, [this](auto& packet) { on_at(packet); });
		publisher.subscribe(packets::Cond::opcode, [this](auto& packet) { on_cond(packet); });
	}

	void SelfPlayer::on_at(std::string_view packet)
	{
		packets::At at_packet(packet);
		id = at_packet.character_id;
		x = at_packet.x;
		y = at_packet.y;
		map_id = at_packet.map_id;
	}

	void SelfPlayer::on_cond(std::string_view packet)
	{
		packets::Cond cond_packet(packet);
		
		if (cond_packet.entity_type == game::EntityType::player && cond_packet.entity_id == this->id) {
			speed = cond_packet.speed;
		}
	}

	Scene::Scene(packets::Publisher& publisher)
	{
		publisher.subscribe(packets::In::opcode, [this](auto& packet) { on_in(packet); });
		publisher.subscribe(packets::CMap::opcode, [this](auto& packet) { on_cmap(packet); });
		publisher.subscribe(packets::Gp::opcode, [this](auto& packet) { on_gp(packet); });
	}

	std::optional<Portal> Scene::find_bazar_portal() const
	{
		auto it = std::find_if(portals.begin(), portals.end(), [](const Portal& portal) {
			return portal.next_map_id == mapids::bazar_map_id;
		});
		
		if (it == portals.end()) {
			return std::nullopt;
		}

		return *it;
	}

	std::optional<Npc> Scene::find_bazar_npc() const
	{
		auto it = std::find_if(npcs.begin(), npcs.end(), [](const std::pair<int, Npc>& npc) {
			return npc.second.vnum == vnums::bazar_npc;
		});

		if (it == npcs.end()) {
			return std::nullopt;
		}

		return it->second;
	}

	void Scene::on_in(std::string_view packet)
	{
		packets::In in_packet(packet);

		switch (in_packet.entity_type) {
		case game::EntityType::npc:
		{
			Npc npc = {
				.id = in_packet.entity_id,
				.x = in_packet.x,
				.y = in_packet.y,
				.vnum = in_packet.vnum.value()
			};

			npcs.emplace(in_packet.entity_id, std::move(npc));
		}

		break;

		default:
			break;
		}
	}

	void Scene::on_cmap(std::string_view packet)
	{
		packets::CMap cmap_packet(packet);

		if (cmap_packet.is_new_map) {
			npcs.clear();
			portals.clear();
		}
	}

	void Scene::on_gp(std::string_view packet)
	{
		packets::Gp gp_packet(packet);

		Portal portal = { 
			.x = gp_packet.x, 
			.y = gp_packet.y, 
			.next_map_id = gp_packet.id 
		};

		portals.emplace_back(portal);
	}

	Sensors::Sensors(packets::Publisher& publisher)
		: scene(std::make_unique<Scene>(publisher))
		, self_player(std::make_unique<SelfPlayer>(publisher))
	{
		
	}

}