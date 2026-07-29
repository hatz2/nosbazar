#include "rc_blist.h"
#include <spdlog/spdlog.h>

#include <strings/encoding.h>

namespace nosbazar::packets {
	nosbazar::packets::RcBlist::ShellEffect::ShellEffect(std::string_view item_data)
	{
		grade = static_cast<ShellGrade>(strings::token<int>(item_data, shell_separator));
		vnum = strings::token<int>(item_data, shell_separator);
		value = strings::token<int>(item_data, shell_separator);
		upgrade = strings::token<int>(item_data, shell_separator);
	}

	nlohmann::json RcBlist::ShellEffect::json() const
	{
		return {
			{"grade", std::to_underlying(grade)},
			{"vnum", vnum},
			{"value", value},
			{"upgrade", upgrade},
		};
	}

	RcBlist::WeaponData::WeaponData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		rare = strings::token<int>(item_data, data_separator);
		upgrade = strings::token<int>(item_data, data_separator);
		fixed_level = strings::token<bool>(item_data, data_separator);
		required_level = strings::token<int>(item_data, data_separator);
		min_dmg = strings::token<int>(item_data, data_separator);
		max_dmg = strings::token<int>(item_data, data_separator);
		hit_rate_concentration = strings::token<int>(item_data, data_separator);
		critical_rate = strings::token<int>(item_data, data_separator);
		critical_dmg = strings::token<int>(item_data, data_separator);
		ammo = strings::token<int>(item_data, data_separator);
		max_ammo = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);
		unknown_3 = strings::token<int>(item_data, data_separator);
		rarity_of_effects = strings::token<int>(item_data, data_separator);
		owner_id = strings::token<int>(item_data, data_separator);
		shell_count = strings::token<int>(item_data, data_separator);
		unknown_6 = strings::token<int>(item_data, data_separator);

		shells.reserve(shell_count);
		for (int i = 0; i < shell_count; ++i) {
			std::string_view shell_effect_data = strings::token<std::string_view>(item_data, data_separator);
			shells.emplace_back(ShellEffect(shell_effect_data));
		}

		unknown_7 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::WeaponData::json() const
	{
		nlohmann::json result = {
			{ "vnum", vnum },
			{ "rare", rare },
			{ "upgrade", upgrade },
			{ "fixed_level", fixed_level },
			{ "required_level", required_level },
			{ "min_dmg", min_dmg },
			{ "max_dmg", max_dmg },
			{ "price", price },
			{ "shell_count", shell_count }
		};

		nlohmann::json json_shells = nlohmann::json::array();
		for (const ShellEffect& shell : shells) {
			json_shells.push_back(shell.json());
		}

		result["shells"] = json_shells;

		return result;
	}

	RcBlist::ArmourData::ArmourData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		rare = strings::token<int>(item_data, data_separator);
		upgrade = strings::token<int>(item_data, data_separator);
		fixed_level = strings::token<bool>(item_data, data_separator);
		required_level = strings::token<int>(item_data, data_separator);
		melee_defence = strings::token<int>(item_data, data_separator);
		ranged_defence = strings::token<int>(item_data, data_separator);
		magic_defence = strings::token<int>(item_data, data_separator);
		dodge = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);
		unknown_1 = strings::token<int>(item_data, data_separator);
		unknown_2 = strings::token<int>(item_data, data_separator);
		unknown_3 = strings::token<int>(item_data, data_separator);
		shell_count = strings::token<int>(item_data, data_separator);
		unknown_4 = strings::token<int>(item_data, data_separator);

		shells.reserve(shell_count);
		for (int i = 0; i < shell_count; ++i) {
			std::string_view shell_effect_data = strings::token<std::string_view>(item_data, data_separator);
			shells.emplace_back(ShellEffect(shell_effect_data));
		}

		unknown_5 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::ArmourData::json() const
	{
		nlohmann::json result = {
			{ "vnum", vnum },
			{ "rare", rare },
			{ "upgrade", upgrade },
			{ "fixed_level", fixed_level },
			{ "required_level", required_level },
			{ "melee_defence", melee_defence },
			{ "ranged_defence", ranged_defence },
			{ "magic_defence", magic_defence },
			{ "dodge", dodge },
			{ "price", price },
		};

		nlohmann::json json_shells = nlohmann::json::array();
		for (const ShellEffect& shell : shells) {
			json_shells.push_back(shell.json());
		}

		result["shells"] = json_shells;

		return result;
	}

	RcBlist::EquipmentData::EquipmentData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		required_level = strings::token<int>(item_data, data_separator);
		melee_defence = strings::token<int>(item_data, data_separator);
		ranged_defence = strings::token<int>(item_data, data_separator);
		magic_defence = strings::token<int>(item_data, data_separator);
		dodge = strings::token<int>(item_data, data_separator);
		fire_res = strings::token<int>(item_data, data_separator);
		water_res = strings::token<int>(item_data, data_separator);
		light_res = strings::token<int>(item_data, data_separator);
		shadow_res = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);
		sum_level = strings::token<int>(item_data, data_separator);
		unknown_1 = strings::token<int>(item_data, data_separator);
		unknown_2 = strings::token<int>(item_data, data_separator);
		unknown_3 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::EquipmentData::json() const
	{
		return {
			{"vnum", vnum },
			{"required_level", required_level },
			{"melee_defence", melee_defence },
			{"ranged_defence", ranged_defence },
			{"magic_defence", magic_defence },
			{"dodge", dodge },
			{"price", price },
		};
	}

	nlohmann::json RcBlist::CellonOption::json() const
	{
		return {
			{ "vnum", vnum },
			{ "level", level },
			{ "value", value }
		};
	}

	RcBlist::AccessoryData::AccessoryData(std::string_view item_data)
	{
		AccessoryData data;
		vnum = strings::token<int>(item_data, data_separator);
		required_level = strings::token<int>(item_data, data_separator);
		max_option_level = strings::token<int>(item_data, data_separator);
		max_option_count = strings::token<int>(item_data, data_separator);
		option_count = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);

		for (int i = 0; i < option_count; ++i) {
			CellonOption cellon_option{
				.vnum = strings::token<int>(item_data, data_separator),
				.level = strings::token<int>(item_data, data_separator),
				.value = strings::token<int>(item_data, data_separator),
			};
			options.emplace_back(cellon_option);
		}

		unknown_1 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::AccessoryData::json() const
	{
		nlohmann::json result = {
			{ "vnum", vnum },
			{ "required_level", required_level },
			{ "max_option_level", max_option_level },
			{ "max_option_count", max_option_count },
			{ "price", price },
		};

		nlohmann::json options_json = nlohmann::json::array();
		for (const CellonOption& cellon : options) {
			options_json.push_back(cellon.json());
		}

		result["options"] = options_json;

		return result;
	}

	RcBlist::SpecialistData::SpecialistData(std::string_view item_data)
	{
		icon_id = strings::token<int>(item_data, data_separator);
		contains_sp = strings::token<bool>(item_data, data_separator);
		vnum = strings::token<int>(item_data, data_separator);
		job_level = strings::token<int>(item_data, data_separator);
		exp_points = strings::token<int>(item_data, data_separator);
		max_exp_points = strings::token<int>(item_data, data_separator);
		upgrade_level = strings::token<int>(item_data, data_separator);
		attack_points = strings::token<int>(item_data, data_separator);
		defence_points = strings::token<int>(item_data, data_separator);
		element_points = strings::token<int>(item_data, data_separator);
		hp_points = strings::token<int>(item_data, data_separator);
		remaining_points = strings::token<int>(item_data, data_separator);
		perfection_level = strings::token<int>(item_data, data_separator);
		fire_res = strings::token<int>(item_data, data_separator);
		water_res = strings::token<int>(item_data, data_separator);
		light_res = strings::token<int>(item_data, data_separator);
		shadow_res = strings::token<int>(item_data, data_separator);
		attack_perf = strings::token<int>(item_data, data_separator);
		defence_perf = strings::token<int>(item_data, data_separator);
		element_perf = strings::token<int>(item_data, data_separator);
		hp_perf = strings::token<int>(item_data, data_separator);
		fire_res_perf = strings::token<int>(item_data, data_separator);
		water_res_perf = strings::token<int>(item_data, data_separator);
		light_res_perf = strings::token<int>(item_data, data_separator);
		shadow_res_perf = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);
	}
	nlohmann::json RcBlist::SpecialistData::json() const
	{
		int level_percentage = 0;
		
		if (max_exp_points) {
			level_percentage  = exp_points * 100 / max_exp_points;
		}

		int new_icon_id = contains_sp ? vnum : icon_id;

		return {
			{ "icon_id", new_icon_id},
			{ "contains_sp", contains_sp},
			{ "vnum", vnum },
			{ "job_level", job_level },
			{ "level_percentage", level_percentage },
			{ "upgrade_level", upgrade_level },
			{ "attack_points", attack_points },
			{ "defence_points", defence_points },
			{ "element_points", element_points },
			{ "hp_points", hp_points },
			{ "remaining_points", remaining_points },
			{ "perfection_level", perfection_level },
			{ "fire_res", fire_res },
			{ "water_res", water_res },
			{ "light_res", light_res },
			{ "shadow_res", shadow_res },
			{ "attack_perf", attack_perf },
			{ "defence_perf", defence_perf },
			{ "element_perf", element_perf },
			{ "hp_perf", hp_perf },
			{ "fire_res_perf", fire_res_perf },
			{ "water_res_perf", water_res_perf },
			{ "light_res_perf", light_res_perf },
			{ "shadow_res_perf", shadow_res_perf },
			{ "price", price}
		};
	}
	RcBlist::ItemDataFieldsFactory& RcBlist::ItemDataFieldsFactory::instance()
	{
		static ItemDataFieldsFactory instance;
		return instance;
	}

	RcBlist::ItemDataFields RcBlist::ItemDataFieldsFactory::create(Key key, std::string_view packet_data)
	{
		if (registry.contains(key)) {
			return registry.at(key)(packet_data);
		}

		return NoData();
	}

	RcBlist::ItemDataFieldsFactory::ItemDataFieldsFactory()
	{
		using namespace game;

		// Weapons
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::adventurer_sword)}] = [](auto data) { return MeleeWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::swordsman_sword)}] = [](auto data) { return MeleeWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::dagger)}] = [](auto data) { return MeleeWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::fist)}] = [](auto data) { return MeleeWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::bow)}] = [](auto data) { return RangedWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::crossbow)}] = [](auto data) { return RangedWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::token)}] = [](auto data) { return RangedWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::spellgun)}] = [](auto data) { return RangedWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::wand)}] = [](auto data) { return MagicWeaponData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::weapon), std::to_underlying(WeaponSubType::partner)}] = [](auto data) { return MagicWeaponData(data); };

		// Armours
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::armour), std::to_underlying(ArmourSubType::adventurer)}] = [](auto data) { return ArmourData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::armour), std::to_underlying(ArmourSubType::mage)}] = [](auto data) { return ArmourData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::armour), std::to_underlying(ArmourSubType::swordsman)}] = [](auto data) { return ArmourData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::armour), std::to_underlying(ArmourSubType::archer)}] = [](auto data) { return ArmourData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::armour), std::to_underlying(ArmourSubType::martial)}] = [](auto data) { return ArmourData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::armour), std::to_underlying(ArmourSubType::partner)}] = [](auto data) { return ArmourData(data); };

		// Equipment
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::equipment), std::to_underlying(EquipmentSubType::hat)}] = [](auto data) { return EquipmentHatMaskData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::equipment), std::to_underlying(EquipmentSubType::mask)}] = [](auto data) { return EquipmentHatMaskData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::equipment), std::to_underlying(EquipmentSubType::glove)}] = [](auto data) { return ResistancesData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::equipment), std::to_underlying(EquipmentSubType::shoe)}] = [](auto data) { return ResistancesData(data); };
		// TODO: Add the rest of equipments

		// Accessories
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::accessory), std::to_underlying(AccessorySubType::necklace)}] = [](auto data) { return AccessoryData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::accessory), std::to_underlying(AccessorySubType::ring)}] = [](auto data) { return AccessoryData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::accessory), std::to_underlying(AccessorySubType::bracelet)}] = [](auto data) { return AccessoryData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::accessory), std::to_underlying(AccessorySubType::amulet)}] = [](auto data) { return AmuletData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::accessory), std::to_underlying(AccessorySubType::fairy)}] = [](auto data) { return FairyData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::accessory), std::to_underlying(AccessorySubType::mini_pet)}] = [](auto data) { return MiniPetData(data); };

		// Holders
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::holder), std::to_underlying(HolderSubType::sp_card_holder)}] = [](auto data) { return SpecialistData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::holder), std::to_underlying(HolderSubType::pet_bead)}] = [](auto data) { return PetBeadData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::holder), std::to_underlying(HolderSubType::mount_bead)}] = [](auto data) { return MountBeadData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::holder), std::to_underlying(HolderSubType::partner_bead)}] = [](auto data) { return PartnerBeadData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::holder), std::to_underlying(HolderSubType::partner_card_holder)}] = [](auto data) { return PartnerSpecialistData(data); };


		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::shell), std::to_underlying(ShellSubType::weapon)}] = [](auto data) { return ShellData(data); };
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::shell), std::to_underlying(ShellSubType::armour)}] = [](auto data) { return ShellData(data); };

	}
	RcBlist::RcBlist(std::string_view packet)
	{
		auto header = strings::token<std::string_view>(packet);

		page_index = strings::token<uint64_t>(packet);

		while (!packet.empty()) {
			std::string_view item_data = strings::token<std::string_view>(packet);
			items.emplace_back(Item(item_data));
		}
	}

	nlohmann::json RcBlist::json() const
	{
		nlohmann::json result;
		result["page_index"] = page_index;
		result["items"] = nlohmann::json::array();

		for (const Item& item : items) {
			result["items"].push_back(item.json());
		}

		return result;
	}

	RcBlist::Item::Item(std::string_view item_data)
	{
		this->auction_id = strings::token<int>(item_data, item_fields_separator);
		this->owner_id = strings::token<int>(item_data, item_fields_separator);
		this->owner_name = strings::token<std::string>(item_data, item_fields_separator);
		this->item_vnum = strings::token<int>(item_data, item_fields_separator);
		this->amount = strings::token<int>(item_data, item_fields_separator);
		this->is_package = strings::token<bool>(item_data, item_fields_separator);
		this->bazar_price = strings::token<int>(item_data, item_fields_separator);
		this->minutes_left = strings::token<int>(item_data, item_fields_separator);
		this->unknown_1 = strings::token<int>(item_data, item_fields_separator);
		this->unknown_2 = strings::token<int>(item_data, item_fields_separator);
		this->rarity = strings::token<int>(item_data, item_fields_separator);
		this->upgrade = strings::token<int>(item_data, item_fields_separator);
		this->unknown_3 = strings::token<int>(item_data, item_fields_separator);
		this->unknown_4 = strings::token<int>(item_data, item_fields_separator);

		// Get the chunk with the additional data that is separated by '^'
		std::string_view additional_data = strings::token<std::string_view>(item_data);
		this->has_data = additional_data != "^0";

		if (this->has_data) {
			this->data.item_type = strings::token<int>(additional_data, data_separator);
			// Get item types and item DAT info from vnum
			io::Item dat_item = io::ItemDatParser::instance().item_data(item_vnum);

			ItemDataFieldsFactory::Key key{
				.inv_tab = dat_item.inventory_tab,
				.item_type = dat_item.item_type,
				.item_subtype = dat_item.item_subtype
			};

			this->data.fields = std::move(ItemDataFieldsFactory::instance().create(key, additional_data));
		}
	}
	nlohmann::json RcBlist::Item::json() const
	{
		nlohmann::json json_data;

		std::visit([&json_data](auto&& data) {
			json_data = data.json();
		}, data.fields);

		nlohmann::json result = {
			{ "owner_name", strings::convert_to_utf8(owner_name, strings::Encoding::windows1250) },
			{ "item_vnum", item_vnum },
			{ "amount", amount },
			{ "is_package", is_package },
			{ "bazar_price", bazar_price },
			{ "minutes_left", minutes_left },
			{ "data", json_data}
		};

		return result;
	}
	nlohmann::json RcBlist::MeleeWeaponData::json() const
	{
		nlohmann::json result = WeaponData::json();

		result["hit_rate"] = hit_rate_concentration;
		result["critical_rate"] = critical_rate;
		result["critical_dmg"] = critical_dmg;

		return result;
	}
	nlohmann::json RcBlist::RangedWeaponData::json() const
	{
		nlohmann::json result = WeaponData::json();

		result["hit_rate"] = hit_rate_concentration;
		result["critical_rate"] = critical_rate;
		result["critical_dmg"] = critical_dmg;
		result["ammo"] = ammo;
		result["max_ammo"] = max_ammo;

		return result;
	}

	nlohmann::json RcBlist::MagicWeaponData::json() const
	{
		nlohmann::json result = WeaponData::json();

		result["concentration"] = hit_rate_concentration;

		return result;
	}

	nlohmann::json RcBlist::ResistancesData::json() const
	{
		nlohmann::json result = EquipmentData::json();

		result["fire_res"] = fire_res;
		result["water_res"] = water_res;
		result["light_res"] = light_res;
		result["shadow_res"] = shadow_res;
		result["sum_level"] = sum_level;

		return result;
	}

	RcBlist::CostumeData::CostumeData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		required_level = strings::token<int>(item_data), data_separator;
		unknown_1 = strings::token<int>(item_data, data_separator);
		unknown_2 = strings::token<int>(item_data, data_separator);
		unknown_3 = strings::token<int>(item_data, data_separator);
		unknown_4 = strings::token<int>(item_data, data_separator);
		unknown_5 = strings::token<int>(item_data, data_separator);
		unknown_6 = strings::token<int>(item_data, data_separator);
		unknown_7 = strings::token<int>(item_data, data_separator);
		unknown_8 = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);
		unknown_9 = strings::token<int>(item_data, data_separator);
		unknown_10 = strings::token<int>(item_data, data_separator);
		costume_remaining_time_in_hours = strings::token<int>(item_data, data_separator);
		unknown_11 = strings::token<int>(item_data, data_separator);
		unknown_12 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::CostumeData::json() const
	{
		return {
			{"vnum", vnum},
			{"required_level", required_level},
			{"price", price},
			{"costume_remaining_time_in_hours", costume_remaining_time_in_hours}
		};
	}

	RcBlist::AmuletData::AmuletData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		required_level = strings::token<int>(item_data, data_separator);
		remaining_time_in_secs = strings::token<int>(item_data, data_separator);
		unknown_1 = strings::token<int>(item_data, data_separator);
		unknown_2 = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);
		unknown_3 = strings::token<int>(item_data, data_separator);
		unknown_4 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::AmuletData::json() const
	{
		return {
			{"vnum", vnum},
			{"required_level", required_level},
			{"price", price},
			{"remaining_time_in_secs", remaining_time_in_secs}
		};
	}

	RcBlist::FairyData::FairyData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		element_type = strings::token<int>(item_data, data_separator);
		fairy_level_percent = strings::token<int>(item_data, data_separator);
		unknown_1 = strings::token<int>(item_data, data_separator);
		unknown_2 = strings::token<int>(item_data, data_separator);
		unknown_3 = strings::token<int>(item_data, data_separator);
		cannot_trade_after_equip = strings::token<bool>(item_data, data_separator);
		unknown_4 = strings::token<int>(item_data, data_separator);
		monster_to_level_up = strings::token<int>(item_data, data_separator);
		unknown_5 = strings::token<int>(item_data, data_separator);
		unknown_6 = strings::token<int>(item_data, data_separator);
		unknown_7 = strings::token<int>(item_data, data_separator);
		unknown_8 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::FairyData::json() const
	{
		return {
			{"vnum", vnum},
			{"element_type", element_type},
			{"fairy_level_percent", fairy_level_percent},
			{"cannot_trade_after_equip", cannot_trade_after_equip},
			{"monster_to_level_up", monster_to_level_up}
		};
	}

	RcBlist::MiniPetData::MiniPetData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		required_level = strings::token<int>(item_data, data_separator);
		remaining_time_in_hours = strings::token<int>(item_data, data_separator);
		unknown_1 = strings::token<int>(item_data, data_separator);
		unknown_2 = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);
		cannot_trade_after_equip = strings::token<bool>(item_data, data_separator);
		unknown_3 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::MiniPetData::json() const
	{
		return {
			{"vnum", vnum},
			{"required_level", required_level},
			{"remaining_time_in_hours", remaining_time_in_hours},
			{"cannot_trade_after_equip", cannot_trade_after_equip},
			{"price", price}
		};
	}

	RcBlist::PetBeadData::PetBeadData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		has_pet_inside = strings::token<bool>(item_data, data_separator);
		pet_vnum = strings::token<int>(item_data, data_separator);

		if (has_pet_inside) {
			level = strings::token<int>(item_data, data_separator);
			exp_points = strings::token<int>(item_data, data_separator);
			max_exp_points = strings::token<int>(item_data, data_separator);
			attack_level = strings::token<int>(item_data, data_separator);
			defence_level = strings::token<int>(item_data, data_separator);
			stars = strings::token<int>(item_data, data_separator);
		}
	}

	nlohmann::json RcBlist::PetBeadData::json() const
	{
		int level_percentage = 0;

		if (max_exp_points) {
			level_percentage = exp_points * 100 / max_exp_points;
		}

		return {
			{"vnum", vnum},
			{"has_pet_inside", has_pet_inside},
			{"pet_vnum", pet_vnum},
			{"level", level},
			{"level_percentage", level_percentage},
			{"attack_level", attack_level},
			{"defence_level", defence_level},
			{"stars", stars},
			{"price", 0},
		};
	}

	RcBlist::MountBeadData::MountBeadData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		has_mount_inside = strings::token<bool>(item_data, data_separator);
		mount_vnum = strings::token<int>(item_data, data_separator);

		if (has_mount_inside) {
			unknown_1 = strings::token<int>(item_data, data_separator);
		}
	}

	nlohmann::json RcBlist::MountBeadData::json() const
	{
		return {
			{"vnum", vnum},
			{"has_mount_inside", has_mount_inside},
			{"mount_vnum", mount_vnum},
			{"do_not_show_description", true},
			{"do_not_show_flags", true}
		};
	}

	RcBlist::PartnerBeadData::PartnerBeadData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		has_partner_inside = strings::token<bool>(item_data, data_separator);
		partner_vnum = strings::token<int>(item_data, data_separator);

		if (has_partner_inside) {
			level = strings::token<int>(item_data, data_separator);
			exp_points = strings::token<int>(item_data, data_separator);
			max_exp_points = strings::token<int>(item_data, data_separator);
			attack_level = strings::token<int>(item_data, data_separator);
			defence_level = strings::token<int>(item_data, data_separator);
			unknown_1 = strings::token<int>(item_data, data_separator);
		}
	}

	nlohmann::json RcBlist::PartnerBeadData::json() const
	{
		int level_percentage = 0;

		if (max_exp_points) {
			level_percentage = exp_points * 100 / max_exp_points;
		}

		return {
			{"vnum", vnum},
			{"has_partner_inside", has_partner_inside},
			{"partner_vnum", partner_vnum},
			{"level", level},
			{"level_percentage", level_percentage},
			{"attack_level", attack_level},
			{"defence_level", defence_level},
			{"price", 0},
		};
	}

	nlohmann::json RcBlist::PartnerSkillData::json() const
	{
		return {
			{"vnum", vnum},
			{"grade", grade}
		};
	}

	RcBlist::PartnerSpecialistData::PartnerSpecialistData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		has_partner_sp_inside = strings::token<bool>(item_data, data_separator);
		partner_sp_vnum = strings::token<int>(item_data, data_separator);

		if (has_partner_sp_inside) {
			element_type = strings::token<int>(item_data, data_separator);

			for (int i = 0; i < PartnerSpecialistData::num_skills; ++i) {
				PartnerSkillData skill_data{ 
					.vnum = strings::token<int>(item_data, data_separator), 
					.grade = strings::token<int>(item_data, data_separator) 
				};

				skills[i] = std::move(skill_data);
			}

			upgrade_level = strings::token<int>(item_data, data_separator);
			attack_bonus = strings::token<int>(item_data, data_separator);
			defence_bonus = strings::token<int>(item_data, data_separator);
			crit_reduction_bonus = strings::token<int>(item_data, data_separator);
			hp_mp_bonus = strings::token<int>(item_data, data_separator);
			fire_res_bonus = strings::token<int>(item_data, data_separator);
			water_res_bonus = strings::token<int>(item_data, data_separator);
			light_res_bonus = strings::token<int>(item_data, data_separator);
			shadow_res_bonus = strings::token<int>(item_data, data_separator);
			unknown_1 = strings::token<int>(item_data, data_separator);
		}
	}

	nlohmann::json RcBlist::PartnerSpecialistData::json() const
	{
		nlohmann::json json_skills = nlohmann::json::array();
		for (const PartnerSkillData& skill : skills) {
			json_skills.push_back(std::move(skill.json()));
		}

		return {
			{"vnum", vnum},
			{"has_partner_sp_inside", has_partner_sp_inside},
			{"partner_sp_vnum", partner_sp_vnum},
			{"element_type", element_type},
			{"skills", json_skills},
			{"upgrade_level", upgrade_level},
			{"attack_bonus", attack_bonus},
			{"defence_bonus", defence_bonus},
			{"crit_reduction_bonus", crit_reduction_bonus},
			{"hp_mp_bonus", hp_mp_bonus},
			{"fire_res_bonus", fire_res_bonus},
			{"water_res_bonus", water_res_bonus},
			{"light_res_bonus", light_res_bonus},
			{"shadow_res_bonus", shadow_res_bonus}
		};
	}

	RcBlist::ShellOptionData::ShellOptionData(std::string_view item_data)
	{
		grade = strings::token<int>(item_data, shell_separator);
		vnum = strings::token<int>(item_data, shell_separator);
		value = strings::token<int>(item_data, shell_separator);
	}

	nlohmann::json RcBlist::ShellOptionData::json() const
	{
		return {
			{"grade", grade},
			{"vnum", vnum},
			{"value", value}
		};
	}

	RcBlist::ShellData::ShellData(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		required_level = strings::token<int>(item_data, data_separator);
		rare = strings::token<int>(item_data, data_separator);
		price = strings::token<int>(item_data, data_separator);

		int number_of_options = strings::token<int>(item_data, data_separator);
		for (int i = 0; i < number_of_options; ++i) {
			std::string_view option = strings::token<std::string_view>(item_data, data_separator);
			ShellOptionData option_data(option);
			options.push_back(std::move(option_data));
		}
		unknown_1 = strings::token<int>(item_data, data_separator);
	}

	nlohmann::json RcBlist::ShellData::json() const
	{
		nlohmann::json json_options = nlohmann::json::array();
		for (const auto& option : options) {
			json_options.push_back(std::move(option.json()));
		}

		return {
			{"vnum", vnum},
			{"required_level", required_level},
			{"rare", rare},
			{"price", price},
			{"shell_options", json_options},
			{"do_not_show_description", true},
		};
	}

}
