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

	enum class InventoryTab : uint16_t {
		equip = 0,
		main = 1,
		etc = 2,
	};

	enum class EquipType : uint16_t {
		weapon = 0,
		armour = 1,
		equipment = 2,
		accessory = 3,
		specialist = 4,
		holder = 5,
		shell = 6
	};

	enum class WeaponSubType : uint16_t {
		adventurer_sword = 0,
		swordsman_sword = 1,
		dagger = 3,
		fist = 4,
		crossbow = 5, // same for adventurer slingshot
		bow = 6,
		spellgun = 8,
		wand = 9,
		token = 11,
		partner = 12
	};

	enum class ArmourSubType : uint16_t {
		adventurer = 0,
		mage = 1,
		archer = 2,
		swordsman = 3,
		partner = 4,
		martial = 5,
	};

	enum class EquipmentSubType : uint16_t {
		hat = 0,
		mask = 1,
		glove = 2,
		shoe = 3,
		costume = 4,
		costume_hat = 5,
		costume_weapon = 6,
		costume_wings = 7,
	};

	enum class AccessorySubType : uint16_t {
		necklace = 0,
		ring = 1,
		bracelet = 2,
		fairy = 3,
		amulet = 4,
		mini_pet = 5
	};

	enum class HolderSubType : uint16_t {
		pet_bead = 0,
		partner_bead = 1,
		sp_card_holder = 2,
		unknown = 3,
		mount_bead = 4,
		fairy_bead = 5,
		partner_card_holder = 6,
	};

	enum class ShellSubType : uint16_t {
		weapon = 0,
		armour = 1,
	};

	enum class MainType : uint16_t {

	};

	enum class EtcType : uint16_t {

	};
}