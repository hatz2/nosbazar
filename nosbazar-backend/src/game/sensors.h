#pragma once

#include <packets/packet_publisher.h>
#include <memory>
#include <unordered_map>

namespace nosbazar::game {
	struct SelfPlayer {
		int id{};
		int dest_x{};
		int dest_y{};
		int x{};
		int y{};
		int map_id{};
		int speed{};

		explicit SelfPlayer(packets::Publisher& publisher);

	private:
		void on_at(std::string_view packet);
		void on_cond(std::string_view packet);

	};

	struct Npc {
		int id{};
		int x{};
		int y{};
		int vnum{};
	};

	struct Portal {
		int x;
		int y;
		int next_map_id;
	};

	struct Scene {
		std::unordered_map<int, Npc> npcs;
		std::vector<Portal> portals;

		explicit Scene(packets::Publisher& publisher);

		std::optional<Portal> find_bazar_portal() const;

	private:
		void on_in(std::string_view packet);
		void on_cmap(std::string_view packet);
		void on_gp(std::string_view packet);
	};

	class Sensors {
	public:
		explicit Sensors(packets::Publisher& publisher);

		std::unique_ptr<Scene> scene;
		std::unique_ptr<SelfPlayer> self_player;
	};
}