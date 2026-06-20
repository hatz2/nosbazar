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
		armour = 1,
		equipment = 2,
		accessory = 3,
		specialist = 4,
		holders = 5,
		shells = 6
	};

	enum class WeaponSubType : uint8_t {
		adventurer_sword = 0,
		swordsman_sword = 1,
		dagger = 3,
		fist = 4,
		slingshot = 5,
		bow = 6,
		spellgun = 8,
		wand = 9,
		token = 11,
		partner_weapons = 12
	};

	enum class ArmourSubType : uint8_t {

	};

	enum class EquipmentSubType : uint8_t {
		hat = 0,
		mask = 1,
		shoe = 2,
		glove = 3,
	};

	enum class AccessorySubType : uint8_t {
		necklace = 0,
		ring = 1,
		bracelet = 2,
		fairy = 3,
		amulet = 4,
	};

	enum class HolderSubType : uint8_t {
		pet_bead = 0,
		partner_bead = 1,
		sp_card_holder = 2,
		unknown = 3,
		mount_bead = 4,
		fairy_bead = 5,
		partner_card_holder = 6,
	};

	enum class ShellSubType : uint8_t {
		weapon = 0,
		armour = 1,
	};

	enum class MainType : uint8_t {

	};

	enum class EtcType : uint8_t {

	};
}