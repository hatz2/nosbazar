#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>
#include <vector>
#include <io/item_dat_parser.h>
#include <game/enums.h>

namespace nosbazar::packets {
	class RcBlist {
	public:
		static inline std::string_view opcode = "rc_blist";

		enum class ShellGrade : uint8_t {
			c = 1,
			b = 2,
			a = 3,
			s = 4,
			special = 7
		};


		struct ShellEffect {
			ShellGrade grade;
			int vnum;
			int value;
			int unknown_3; // maybe upgradable shell effect like (+6)?
		};

		struct WeaponDefaultData {
			int vnum;
			int rare;
			int upgrade;
			bool fixed_level;
			int required_level;
			int min_dmg;
			int max_dmg;
			int hit_rate_concentration; // concentration only on wands
			int critical_rate;
			int critical_dmg;
			int ammo; // only if weapon is bow or xbow
			int max_ammo; // only if weapon is bow or xbox
			int price;
			int unknown_3;
			int unknown_4;
			int unknown_5;
			int shell_count;
			int unknown_6;
			std::vector<ShellEffect> shells;
			int unknown_7;
		};

		struct MeleeWeaponData : WeaponDefaultData {

		};

		struct RangedWeaponData : WeaponDefaultData {

		};

		struct MagicWeaponData : WeaponDefaultData {
			
		};

		struct ArmourData {
			int vnum;
			int rare;
			int upgrade;
			bool fixed_level;
			int required_level;
			int melee_defence;
			int ranged_defence;
			int magic_defence;
			int dodge;
			int price;
			int unknown_1;
			int unknown_2;
			int unknown_3;
			int shell_count;
			int unknown_4;
			std::vector<ShellEffect> shells;
			int unknown_5;
		};

		struct EquipmentData {
			int vnum;
			int required_level;
			int melee_defence;
			int ranged_defence;
			int magic_defence;
			int dodge;
			int fire_res;
			int water_res;
			int light_res;
			int shadow_res;
			int price;
			int sum_level;
			int unknown_1;
			int unknown_2;
			int unknown_3;
		};

		struct CellonOption {
			int vnum;
			int level;
			int value;
		};

		// TODO: Fix this struct
		struct AccessoryData {
			int vnum;
			int required_level;
			int max_option_level;
			int max_option_count;
			int option_count;
			int price;
			std::vector<CellonOption> options;
			int unknown_1;
		};

		struct SpecialistData {
			int icon_id;
			bool contains_sp;
			int vnum;
			int job_level;
			int exp_points;
			int max_exp_points;
			int upgrade_level;
			int attack_points;
			int defence_points;
			int element_points;
			int hp_points;
			int remaining_points;
			int perfection_level;
			int fire_res;
			int water_res;
			int light_res;
			int shadow_res;
			int attack_perf;
			int defence_perf;
			int element_perf;
			int hp_perf;
			int fire_res_perf;
			int water_res_perf;
			int light_res_perf;
			int shadow_res_perf;
			int unknown_1;
		};

		using ItemDataFields = std::variant<
			WeaponDefaultData, 
			ArmourData, 
			EquipmentData, 
			AccessoryData, 
			SpecialistData
		>;

		enum class ItemDataClass {
			melee_weapon = 0,
			ranged_weapon = 1,
			armour = 2,
			equipment = 3,
			accessory = 4,
			magic_weapon = 5,
			specialist = 7,
			raid_boxes = 8
		};

		struct ItemData {
			// on weapons this field might be the dmg type (0 = melee, 1 = ranged, 5 = magic)
			int item_type;
			ItemDataFields fields;
		};

		struct Item {
			int auction_id;
			int owner_id;
			std::string owner_name;
			int item_vnum;
			int amount;
			bool is_package;
			uint64_t bazar_price;
			uint64_t minutes_left;
			int unknown_1;
			int unknown_2;
			int rarity;
			int upgrade;
			int unknown_3;
			int unknown_4;
			bool has_data;
			ItemData data;
		};

		uint64_t page_index;
		std::vector<Item> items;

		explicit RcBlist(std::string_view packet) {
			auto header = strings::token<std::string_view>(packet);

			page_index = strings::token<uint64_t>(packet);
			
			while (!packet.empty()) {
				Item item;

				// Get the chunk of data for 1 single item
				std::string_view item_data = strings::token<std::string_view>(packet);

				item.auction_id = strings::token<int>(item_data, item_fields_separator);
				item.owner_id = strings::token<int>(item_data, item_fields_separator);
				item.owner_name = strings::token<std::string>(item_data, item_fields_separator);
				item.item_vnum = strings::token<int>(item_data, item_fields_separator);
				item.amount = strings::token<int>(item_data, item_fields_separator);
				item.is_package = strings::token<bool>(item_data, item_fields_separator);
				item.bazar_price = strings::token<uint64_t>(item_data, item_fields_separator);
				item.minutes_left = strings::token<uint64_t>(item_data, item_fields_separator);
				item.unknown_1 = strings::token<int>(item_data, item_fields_separator);
				item.unknown_2 = strings::token<int>(item_data, item_fields_separator);
				item.rarity = strings::token<int>(item_data, item_fields_separator);
				item.upgrade = strings::token<int>(item_data, item_fields_separator);
				item.unknown_3 = strings::token<int>(item_data, item_fields_separator);
				item.unknown_4 = strings::token<int>(item_data, item_fields_separator);

				// Get the chunk with the additional data that is separated by '^'
				std::string_view additional_data = strings::token<std::string_view>(item_data);
				item.data.item_type = strings::token<int>(additional_data, data_separator);
				item.has_data = additional_data != "^0";

				// Get item types and item DAT info from vnum
				io::Item dat_item = io::ItemDatParser::instance().item_data(item.item_vnum);
				game::InventoryTab inv_tab = static_cast<game::InventoryTab>(dat_item.inventory_tab);
				
				// We are only interested in items from EQUIP tab as those are the ones
				// that contains some useful data
				if (inv_tab == game::InventoryTab::equip) {
					game::EquipType item_type = static_cast<game::EquipType>(dat_item.item_type);

					switch (item_type) {
					case game::EquipType::weapon:
						item.data.fields = std::move(parse_default_weapon(additional_data));
						break;

					case game::EquipType::armour:
						item.data.fields = std::move(parse_armour(additional_data));
						break;

					case game::EquipType::equipment:
						switch (static_cast<game::EquipmentSubType>(dat_item.item_subtype)) {
						case game::EquipmentSubType::hat:
						case game::EquipmentSubType::mask:
						case game::EquipmentSubType::glove:
						case game::EquipmentSubType::shoe:
							item.data.fields = std::move(parse_equipment(additional_data));
							break;
						}
						default:
							break;

						break;

					case game::EquipType::accessory:
						switch (static_cast<game::AccessorySubType>(dat_item.item_subtype)) {
						case game::AccessorySubType::necklace:
						case game::AccessorySubType::ring:
						case game::AccessorySubType::bracelet:
							item.data.fields = std::move(parse_accessory(additional_data));
							break;
						// TODO: Add fairy, amulet, costumes
						default:
							break;
						}
						break;

					case game::EquipType::holders:
						switch (static_cast<game::HolderSubType>(dat_item.item_subtype)) {
						case game::HolderSubType::sp_card_holder:
							break;
						// TODO: Add the rest of holders parsing
						default:
							break;
						}
						break;

					case game::EquipType::shells:
						break;
					}



					// default weapon
					//if (item.data.item_type == 0) {
					//	item.data.fields = std::move(parse_default_weapon(additional_data));
					//}

					// bow
					//if (item.data.item_type == 1) {
					//	item.data.fields = std::move(parse_default_weapon(additional_data));
					//}

					// armour
					//if (item.data.item_type == 2) {
					//	item.data.fields = std::move(parse_armour(additional_data));
					//}

					// hat, mask, gloves, shoes
					//if (item.data.item_type == 3) {
					//	item.data.fields = std::move(parse_equipment(additional_data));
					//}

					// accessories: necklace, ring, bracelet, fairy, amulet, mini pet
					if (item.data.item_type == 4) {
						item.data.fields = std::move(parse_accessory(additional_data));
					}

					// wands
					if (item.data.item_type == 5) {
						item.data.fields = std::move(parse_default_weapon(additional_data));
					}

					// specialists
					if (item.data.item_type == 7) {
						item.data.fields = std::move(parse_specialist(additional_data));
					}
				}
				else {
					SPDLOG_DEBUG("Item has data but it's not an EQUIP item. Item vnum: {}", item.item_vnum);
				}
			}
		}

	private:
		static constexpr char item_fields_separator = '|';
		static constexpr char data_separator = '^';
		static constexpr char shell_separator = '.';

		WeaponDefaultData parse_default_weapon(std::string_view item_data) {
			WeaponDefaultData data;
			data.vnum = strings::token<int>(item_data, data_separator);
			data.rare = strings::token<int>(item_data, data_separator);
			data.upgrade = strings::token<int>(item_data, data_separator);
			data.fixed_level = strings::token<bool>(item_data, data_separator);
			data.required_level = strings::token<int>(item_data, data_separator);
			data.min_dmg = strings::token<int>(item_data, data_separator);
			data.max_dmg = strings::token<int>(item_data, data_separator);
			data.hit_rate_concentration = strings::token<int>(item_data, data_separator);
			data.critical_rate = strings::token<int>(item_data, data_separator);
			data.critical_dmg = strings::token<int>(item_data, data_separator);
			data.ammo = strings::token<int>(item_data, data_separator);
			data.max_ammo = strings::token<int>(item_data, data_separator);
			data.price = strings::token<int>(item_data, data_separator);
			data.unknown_3 = strings::token<int>(item_data, data_separator);
			data.unknown_4 = strings::token<int>(item_data, data_separator);
			data.unknown_5 = strings::token<int>(item_data, data_separator);
			data.shell_count = strings::token<int>(item_data, data_separator);
			data.unknown_6 = strings::token<int>(item_data, data_separator);

			data.shells.reserve(data.shell_count);
			for (int i = 0; i < data.shell_count; ++i) {
				// Get the chunk with the shell effect string data
				std::string_view shell_effect = strings::token<std::string_view>(item_data, data_separator);
				ShellEffect effect;
				effect.grade = static_cast<ShellGrade>(strings::token<int>(shell_effect, shell_separator));
				effect.vnum = strings::token<int>(shell_effect, shell_separator);
				effect.value = strings::token<int>(shell_effect, shell_separator);
				effect.unknown_3 = strings::token<int>(shell_effect, shell_separator);
				data.shells.push_back(std::move(effect));
			}

			data.unknown_7 = strings::token<int>(item_data, data_separator);

			return data;
		}

		ArmourData parse_armour(std::string_view item_data) {
			ArmourData data;
			data.vnum = strings::token<int>(item_data, data_separator);
			data.rare = strings::token<int>(item_data, data_separator);
			data.upgrade = strings::token<int>(item_data, data_separator);
			data.fixed_level = strings::token<bool>(item_data, data_separator);
			data.required_level = strings::token<int>(item_data, data_separator);
			data.melee_defence = strings::token<int>(item_data, data_separator);
			data.ranged_defence = strings::token<int>(item_data, data_separator);
			data.magic_defence = strings::token<int>(item_data, data_separator);
			data.dodge = strings::token<int>(item_data, data_separator);
			data.price = strings::token<int>(item_data, data_separator);
			data.unknown_1 = strings::token<int>(item_data, data_separator);
			data.unknown_2= strings::token<int>(item_data, data_separator);
			data.unknown_3 = strings::token<int>(item_data, data_separator);
			data.shell_count = strings::token<int>(item_data, data_separator);
			data.unknown_4 = strings::token<int>(item_data, data_separator);

			data.shells.reserve(data.shell_count);
			for (int i = 0; i < data.shell_count; ++i) {
				// Get the chunk with the shell effect string data
				std::string_view shell_effect = strings::token<std::string_view>(item_data, data_separator);
				ShellEffect effect;
				effect.grade = static_cast<ShellGrade>(strings::token<int>(shell_effect, shell_separator));
				effect.vnum = strings::token<int>(shell_effect, shell_separator);
				effect.value = strings::token<int>(shell_effect, shell_separator);
				effect.unknown_3 = strings::token<int>(shell_effect, shell_separator);
				data.shells.push_back(std::move(effect));
			}

			data.unknown_5 = strings::token<int>(item_data, data_separator);

			return data;
		}

		EquipmentData parse_equipment(std::string_view item_data) {
			EquipmentData data;

			data.vnum = strings::token<int>(item_data, data_separator);
			data.required_level = strings::token<int>(item_data, data_separator);
			data.melee_defence = strings::token<int>(item_data, data_separator);
			data.ranged_defence = strings::token<int>(item_data, data_separator);
			data.magic_defence = strings::token<int>(item_data, data_separator);
			data.dodge = strings::token<int>(item_data, data_separator);
			data.fire_res = strings::token<int>(item_data, data_separator);
			data.water_res = strings::token<int>(item_data, data_separator);
			data.light_res = strings::token<int>(item_data, data_separator);
			data.shadow_res = strings::token<int>(item_data, data_separator);
			data.price = strings::token<int>(item_data, data_separator);
			data.sum_level = strings::token<int>(item_data, data_separator);
			data.unknown_1 = strings::token<int>(item_data, data_separator);
			data.unknown_2 = strings::token<int>(item_data, data_separator);
			data.unknown_3 = strings::token<int>(item_data, data_separator);

			return data;
		}

		AccessoryData parse_accessory(std::string_view item_data) {
			AccessoryData data;
			data.vnum = strings::token<int>(item_data, data_separator);
			data.required_level = strings::token<int>(item_data, data_separator);
			data.max_option_level = strings::token<int>(item_data, data_separator);
			data.max_option_count = strings::token<int>(item_data, data_separator);
			data.option_count = strings::token<int>(item_data, data_separator);
			data.price = strings::token<int>(item_data, data_separator);

			for (int i = 0; i < data.option_count; ++i) {
				CellonOption option;
				option.vnum = strings::token<int>(item_data, data_separator);
				option.level = strings::token<int>(item_data, data_separator);
				option.value = strings::token<int>(item_data, data_separator);
				data.options.push_back(std::move(option));
			}

			data.unknown_1 = strings::token<int>(item_data, data_separator);
			return data;
		}

		SpecialistData parse_specialist(std::string_view item_data) {
			SpecialistData data;

			data.icon_id = strings::token<int>(item_data, data_separator);
			data.contains_sp = strings::token<bool>(item_data, data_separator);
			data.vnum = strings::token<int>(item_data, data_separator);
			data.job_level = strings::token<int>(item_data, data_separator);
			data.exp_points = strings::token<int>(item_data, data_separator);
			data.max_exp_points = strings::token<int>(item_data, data_separator);
			data.upgrade_level = strings::token<int>(item_data, data_separator);
			data.attack_points = strings::token<int>(item_data, data_separator);
			data.defence_points = strings::token<int>(item_data, data_separator);
			data.element_points = strings::token<int>(item_data, data_separator);
			data.hp_points = strings::token<int>(item_data, data_separator);
			data.remaining_points = strings::token<int>(item_data, data_separator);
			data.perfection_level = strings::token<int>(item_data, data_separator);
			data.fire_res = strings::token<int>(item_data, data_separator);
			data.water_res = strings::token<int>(item_data, data_separator);
			data.light_res = strings::token<int>(item_data, data_separator);
			data.shadow_res = strings::token<int>(item_data, data_separator);
			data.attack_perf = strings::token<int>(item_data, data_separator);
			data.defence_perf = strings::token<int>(item_data, data_separator);
			data.element_perf = strings::token<int>(item_data, data_separator);
			data.hp_perf = strings::token<int>(item_data, data_separator);
			data.fire_res_perf = strings::token<int>(item_data, data_separator);
			data.water_res_perf = strings::token<int>(item_data, data_separator);
			data.light_res_perf = strings::token<int>(item_data, data_separator);
			data.shadow_res_perf = strings::token<int>(item_data, data_separator);
			data.unknown_1 = strings::token<int>(item_data, data_separator);
			
			return data;
		}
	};
}