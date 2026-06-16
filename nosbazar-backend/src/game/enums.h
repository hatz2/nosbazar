#pragma once

#include <cstdint>

namespace nosbazar::game {
	enum class EntityType {
		player = 1,
		npc = 2,
		monster = 3,
		item = 4,
		object = 9
	};

	enum class InventoryTab : uint8_t {
		equip = 0,
		main = 1,
		etc = 2,
	};

	enum class EquipType : uint8_t {
		weapon = 0,
		armor = 1,
		equipment = 2,
		accessory = 3,
		specialist = 4,
		holders = 5,
		shells = 6
	};

	enum class MainType : uint8_t {

	};

	enum class EtcType : uint8_t {

	};
}