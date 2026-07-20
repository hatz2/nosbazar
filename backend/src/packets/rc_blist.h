#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>
#include <vector>
#include <io/item_dat_parser.h>
#include <nlohmann/json.hpp>
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
			ShellGrade grade{};
			int vnum{};
			int value{};
			int upgrade{};

			ShellEffect() = default;
			ShellEffect(std::string_view item_data);

			nlohmann::json json() const;
		};

		struct WeaponData {
			int vnum{};
			int rare{};
			int upgrade{};
			bool fixed_level{};
			int required_level{};
			int min_dmg{};
			int max_dmg{};
			int hit_rate_concentration{}; // concentration only on wands
			int critical_rate{};
			int critical_dmg{};
			int ammo{}; // only if weapon is bow or xbow
			int max_ammo{}; // only if weapon is bow or xbox
			int price{};
			int unknown_3{};
			int unknown_4{};
			int unknown_5{};
			int shell_count{};
			int unknown_6{};
			std::vector<ShellEffect> shells;
			int unknown_7{};

			WeaponData() = default;
			WeaponData(std::string_view item_data);

			virtual nlohmann::json json() const;
		};

		struct MeleeWeaponData : public WeaponData {
			MeleeWeaponData(std::string_view data) : WeaponData(data) {}

			nlohmann::json json() const override;
		};

		struct RangedWeaponData : public WeaponData {
			RangedWeaponData(std::string_view data) : WeaponData(data) {}

			nlohmann::json json() const override;
		};

		struct MagicWeaponData : public WeaponData {
			MagicWeaponData(std::string_view data) : WeaponData(data) {}

			nlohmann::json json() const override;
		};

		struct ArmourData {
			int vnum{};
			int rare{};
			int upgrade{};
			bool fixed_level{};
			int required_level{};
			int melee_defence{};
			int ranged_defence{};
			int magic_defence{};
			int dodge{};
			int price{};
			int unknown_1{};
			int unknown_2{};
			int unknown_3{};
			int shell_count{};
			int unknown_4{};
			std::vector<ShellEffect> shells;
			int unknown_5{};

			ArmourData() = default;
			ArmourData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct EquipmentData {
			int vnum{};
			int required_level{};
			int melee_defence{};
			int ranged_defence{};
			int magic_defence{};
			int dodge{};
			int fire_res{};
			int water_res{};
			int light_res{};
			int shadow_res{};
			int price{};
			int sum_level{};
			int unknown_1{};
			int unknown_2{};
			int unknown_3{};
			
			EquipmentData() = default;
			EquipmentData(std::string_view item_data);
			virtual nlohmann::json json() const;
		};

		struct CostumeData {
			int vnum{};
			int required_level{};
			int unknown_1{};
			int unknown_2{};
			int unknown_3{};
			int unknown_4{};
			int unknown_5{};
			int unknown_6{};
			int unknown_7{};
			int unknown_8{};
			int price{};
			int unknown_9{};
			int unknown_10{};
			int costume_remaining_time_in_hours{};
			int unknown_11{};
			int unknown_12{};

			CostumeData() = default;
			CostumeData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct AmuletData {
			int vnum{};
			int required_level{};
			int remaining_time_in_secs{};
			int unknown_1{};
			int unknown_2{};
			int price{};
			int unknown_3{};
			int unknown_4{};

			AmuletData() = default;
			AmuletData(std::string_view item_data);
			nlohmann::json json() const;
		};

		// Gloves and shoes
		struct ResistancesData : EquipmentData {
			ResistancesData(std::string_view item_data) : EquipmentData(item_data) {};
			nlohmann::json json() const override;
		};

		// Mask and hat
		struct EquipmentHatMaskData : EquipmentData {
			EquipmentHatMaskData(std::string_view item_data) : EquipmentData(item_data) {};
		};

		struct CellonOption {
			int vnum{};
			int level{};
			int value{};

			nlohmann::json json() const;
		};

		// TODO: Fix this struct
		struct AccessoryData {
			int vnum{};
			int required_level{};
			int max_option_level{};
			int max_option_count{};
			int option_count{};
			int price{};
			std::vector<CellonOption> options;
			int unknown_1{};

			AccessoryData() = default;
			AccessoryData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct SpecialistData {
			int icon_id{};
			bool contains_sp{};
			int vnum{};
			int job_level{};
			int exp_points{};
			int max_exp_points{};
			int upgrade_level{};
			int attack_points{};
			int defence_points{};
			int element_points{};
			int hp_points{};
			int remaining_points{};
			int perfection_level{};
			int fire_res{};
			int water_res{};
			int light_res{};
			int shadow_res{};
			int attack_perf{};
			int defence_perf{};
			int element_perf{};
			int hp_perf{};
			int fire_res_perf{};
			int water_res_perf{};
			int light_res_perf{};
			int shadow_res_perf{};
			int price{};

			SpecialistData() = default;
			SpecialistData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct FairyData {
			int vnum{};
			int element_type{};
			int fairy_level_percent{};
			int unknown_1{};
			int unknown_2{};
			int unknown_3{};
			bool cannot_trade_after_equip{};
			int unknown_4{};
			int monster_to_level_up{};
			int unknown_5{};
			int unknown_6{};
			int unknown_7{};
			int unknown_8{};

			FairyData() = default;
			FairyData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct MiniPetData {
			int vnum{};
			int required_level{};
			int remaining_time_in_hours{};
			int unknown_1{};
			int unknown_2{};
			int price{};
			bool cannot_trade_after_equip{};
			int unknown_3{};

			MiniPetData() = default;
			MiniPetData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct PetBeadData {
			int vnum{};
			bool has_pet_inside{};
			int pet_vnum{};
			int level{};
			int exp_points{};
			int max_exp_points{};
			int attack_level{};
			int defence_level{};
			int stars{};

			PetBeadData() = default;
			PetBeadData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct MountBeadData {
			int vnum{};
			bool has_mount_inside{};
			int mount_vnum{};
			int unknown_1{};

			MountBeadData() = default;
			MountBeadData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct PartnerBeadData {
			int vnum{};
			bool has_partner_inside{};
			int partner_vnum{};
			int level{};
			int exp_points{};
			int max_exp_points{};
			int attack_level{};
			int defence_level{};
			int unknown_1{};

			PartnerBeadData() = default;
			PartnerBeadData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct PartnerSkillData {
			int vnum{};
			int grade{};

			nlohmann::json json() const;
		};

		struct PartnerSpecialistData {
			static constexpr int num_skills = 3;

			int vnum{};
			bool has_partner_sp_inside{};
			int partner_sp_vnum{};
			int element_type{};
			std::array<PartnerSkillData, num_skills> skills;
			int upgrade_level{};
			int attack_bonus{};
			int defence_bonus{};
			int crit_reduction_bonus{};
			int hp_mp_bonus{};
			int fire_res_bonus{};
			int water_res_bonus{};
			int light_res_bonus{};
			int shadow_res_bonus{};
			int unknown_1{};

			PartnerSpecialistData() = default;
			PartnerSpecialistData(std::string_view item_data);
			nlohmann::json json() const;
		};

		struct NoData {
			nlohmann::json json() const {
				return nlohmann::json::object();
			}
		};

		using ItemDataFields = std::variant<
			NoData,
			MeleeWeaponData,
			RangedWeaponData,
			MagicWeaponData,
			ArmourData, 
			ResistancesData,
			EquipmentHatMaskData,
			AccessoryData, 
			SpecialistData,
			CostumeData,
			AmuletData,
			FairyData,
			MiniPetData,
			PetBeadData,
			MountBeadData,
			PartnerBeadData,
			PartnerSpecialistData
		>;

		/**
		 * @brief Singleton factory to create structs from parsing the data field of the packet
		 * from the item's inventory tab, type and subtype.
		 */
		class ItemDataFieldsFactory {
		public:
			struct Key {
				uint16_t inv_tab{};
				uint16_t item_type{};
				uint16_t item_subtype{};

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
			int item_type{};
			ItemDataFields fields;
		};

		/**
		 * @brief This struct represents one item from the rc_blist packet.
		 */
		struct Item {
			int auction_id{};
			int owner_id{};
			std::string owner_name{};
			int item_vnum{};
			int amount{};
			bool is_package{};
			int bazar_price{};
			int minutes_left{};
			int unknown_1{};
			int unknown_2{};
			int rarity{};
			int upgrade{};
			int unknown_3{};
			int unknown_4{};
			bool has_data{};
			ItemData data;

			Item(std::string_view item_data);
			nlohmann::json json() const;
		};

		explicit RcBlist(std::string_view packet);

		inline uint64_t get_page_index() const {
			return page_index;
		};

		inline const std::vector<Item>& get_items() const {
			return items;
		};

		nlohmann::json json() const;

	private:
		static constexpr char item_fields_separator = '|';
		static constexpr char data_separator = '^';
		static constexpr char shell_separator = '.';

		uint64_t page_index{};
		std::vector<Item> items;
	};
}