#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>
#include <vector>
#include <io/item_dat_parser.h>
#include <game/enums.h>
#include <functional>



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

			ShellEffect() = default;
			ShellEffect(std::string_view item_data);
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

			WeaponDefaultData() = default;

			WeaponDefaultData(std::string_view item_data);
		};

		struct MeleeWeaponData : WeaponDefaultData {
			MeleeWeaponData(std::string_view data) : WeaponDefaultData(data) {}
		};

		struct RangedWeaponData : WeaponDefaultData {
			RangedWeaponData(std::string_view data) : WeaponDefaultData(data) {}
		};

		struct MagicWeaponData : WeaponDefaultData {
			MagicWeaponData(std::string_view data) : WeaponDefaultData(data) {}
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

			ArmourData() = default;

			ArmourData(std::string_view item_data);
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
			
			EquipmentData() = default;

			EquipmentData(std::string_view item_data);
		};

		// Gloves and shoes
		struct ResistancesData : EquipmentData {
		};

		// Mask and hat
		struct EquipmentHatMaskData : EquipmentData {
		};

		struct CellonOption {
			int vnum;
			int level;
			int value;

			CellonOption() = default;

			CellonOption(std::string_view item_data);
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

			AccessoryData() = default;
			AccessoryData(std::string_view item_data);
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

			SpecialistData() = default;
			SpecialistData(std::string_view item_data);
		};

		using ItemDataFields = std::variant<
			WeaponDefaultData, 
			ArmourData, 
			EquipmentData, 
			AccessoryData, 
			SpecialistData
		>;

		/**
		 * @brief Singleton factory to create structs from parsing the data field of the packet
		 * from the item's inventory tab, type and subtype.
		 */
		class ItemDataFieldsFactory {
		public:
			struct Key {
				uint16_t inv_tab;
				uint16_t item_type;
				uint16_t item_subtype;

				bool operator==(const Key&) const = default;
			};

			static ItemDataFieldsFactory& instance();

			ItemDataFields create(Key key, std::string_view packet_data);
		private:
			
			ItemDataFieldsFactory();
			ItemDataFieldsFactory(ItemDataFieldsFactory&) = delete;
			void operator=(ItemDataFieldsFactory&) = delete;

			struct KeyHash {
				inline size_t operator()(const Key& k) const noexcept {
					return (static_cast<size_t>(k.inv_tab) << 32) ^ (static_cast<size_t>(k.item_type) << 16) ^ static_cast<size_t>(k.item_subtype);
				}
			};

			using FactoryFn = std::function<ItemDataFields(std::string_view)>;
			std::unordered_map<Key, FactoryFn, KeyHash> registry;
		};

		/**
		 * @brief This struct represents the chunk of the packet that contains
		 * additional info about the item data like dmg for weapons, shells, etc.
		 */
		struct ItemData {
			int item_type;
			ItemDataFields fields;
		};

		/**
		 * @brief This struct represents one item from the rc_blist packet.
		 */
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

			Item(std::string_view item_data);
		};

		explicit RcBlist(std::string_view packet);

		inline uint64_t get_page_index() const {
			return page_index;
		};

		inline const std::vector<Item>& get_items() const {
			return items;
		};

	private:
		static constexpr char item_fields_separator = '|';
		static constexpr char data_separator = '^';
		static constexpr char shell_separator = '.';

		uint64_t page_index;
		std::vector<Item> items;
	};
}