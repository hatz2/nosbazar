#include "rc_blist.h"
#include <spdlog/spdlog.h>


namespace nosbazar::packets {
	nosbazar::packets::RcBlist::ShellEffect::ShellEffect(std::string_view item_data)
	{
		grade = static_cast<ShellGrade>(strings::token<int>(item_data, shell_separator));
		vnum = strings::token<int>(item_data, shell_separator);
		value = strings::token<int>(item_data, shell_separator);
		unknown_3 = strings::token<int>(item_data, shell_separator);
	}

	RcBlist::WeaponDefaultData::WeaponDefaultData(std::string_view item_data)
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
		unknown_4 = strings::token<int>(item_data, data_separator);
		unknown_5 = strings::token<int>(item_data, data_separator);
		shell_count = strings::token<int>(item_data, data_separator);
		unknown_6 = strings::token<int>(item_data, data_separator);

		shells.reserve(shell_count);
		for (int i = 0; i < shell_count; ++i) {
			std::string_view shell_effect_data = strings::token<std::string_view>(item_data, data_separator);
			shells.emplace_back(ShellEffect(shell_effect_data));
		}

		unknown_7 = strings::token<int>(item_data, data_separator);
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

	RcBlist::CellonOption::CellonOption(std::string_view item_data)
	{
		vnum = strings::token<int>(item_data, data_separator);
		level = strings::token<int>(item_data, data_separator);
		value = strings::token<int>(item_data, data_separator);
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
			options.emplace_back(CellonOption(item_data));
		}

		unknown_1 = strings::token<int>(item_data, data_separator);
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
		unknown_1 = strings::token<int>(item_data, data_separator);
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

		return {};
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
		// TODO: Add the rest of accessory types (amulet, fairy, minipet)

		// Holders
		registry[{std::to_underlying(InventoryTab::equip), std::to_underlying(EquipType::holder), std::to_underlying(HolderSubType::sp_card_holder)}] = [](auto data) { return SpecialistData(data); };
		// TODO: Add the rest of holder types

		// TODO: Add shells
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

	RcBlist::Item::Item(std::string_view item_data)
	{
		this->auction_id = strings::token<int>(item_data, item_fields_separator);
		this->owner_id = strings::token<int>(item_data, item_fields_separator);
		this->owner_name = strings::token<std::string>(item_data, item_fields_separator);
		this->item_vnum = strings::token<int>(item_data, item_fields_separator);
		this->amount = strings::token<int>(item_data, item_fields_separator);
		this->is_package = strings::token<bool>(item_data, item_fields_separator);
		this->bazar_price = strings::token<uint64_t>(item_data, item_fields_separator);
		this->minutes_left = strings::token<uint64_t>(item_data, item_fields_separator);
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
}
