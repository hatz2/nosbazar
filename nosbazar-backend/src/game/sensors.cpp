#include "sensors.h"
#include <packets/in.h>
#include <packets/c_map.h>
#include <packets/gp_packet.h>
#include <spdlog/spdlog.h>
#include "constants.h"

namespace nosbazar::game {
	SelfPlayer::SelfPlayer(packets::Publisher& publisher)
	{
		
	}

	void SelfPlayer::on_at(std::string_view packet)
	{
	}

	void SelfPlayer::on_cond(std::string_view packet)
	{

	}

	Scene::Scene(packets::Publisher& publisher)
	{
		publisher.subscribe(packets::In::opcode, [this](const std::string& packet) { on_in(packet); });
		publisher.subscribe(packets::CMap::opcode, [this](const std::string& packet) { on_cmap(packet); });
		publisher.subscribe(packets::Gp::opcode, [this](const std::string& packet) { on_gp(packet); });
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