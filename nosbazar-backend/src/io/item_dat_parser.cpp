#include "item_dat_parser.h"
#include <sstream>
#include <game/enums.h>
#include <utility>

nosbazar::io::ItemDatParser& nosbazar::io::ItemDatParser::instance()
{
	static ItemDatParser instance;
	return instance;
}

void nosbazar::io::ItemDatParser::parse(const std::string& file_content)
{
	std::istringstream iss(file_content);

	std::string key;

	Item item;
	while (iss >> key && key != "~") {
		if (key == "VNUM") {
			iss >> item.vnum;
			iss >> item.price;
		}
		else if (key == "NAME") {
			iss >> item.item_name_code;
		}
		else if (key == "INDEX") {
			iss >> item.inventory_tab;
			item.inventory_tab %= 4;
			iss >> item.item_type;
			iss >> item.item_subtype;
			iss >> item.equip_slot;
			iss >> item.icon_id;
			iss >> item.visual_change_id;
		}
		else if (key == "TYPE") {
			iss >> item.attack_type;
			iss >> item.required_class;
		}
		else if (key == "FLAG") {
			iss >> item.flags.unknown_1;
			iss >> item.flags.unknown_2;
			iss >> item.flags.unknown_3;
			iss >> item.flags.no_selling;
			iss >> item.flags.no_dropping;
			iss >> item.flags.no_trading;
			iss >> item.flags.miniland_item;
			iss >> item.flags.miniland_item_2;
			iss >> item.flags.show_warning_on_use;
			iss >> item.flags.is_time_space_reward_box;
			iss >> item.flags.show_description_on_hover;
			iss >> item.flags.unknown_4;
			iss >> item.flags.follow_mouse_on_use;
			iss >> item.flags.show_something_on_hover;
			iss >> item.flags.can_be_colored;
			iss >> item.flags.female_can_wear;
			iss >> item.flags.male_can_wear;
			iss >> item.flags.unknown_5;
			iss >> item.flags.play_sound_on_pickup;
			iss >> item.flags.use_reputation_as_price;
			iss >> item.flags.is_champion_equip;
			iss >> item.flags.unknown_6;
			iss >> item.flags.is_limited;
			iss >> item.flags.unknown_7;
			iss >> item.flags.unknown_8;
		}
		else if (key == "DATA") {
			for (size_t i = 0; i < item.data.size(); ++i) {
				iss >> item.data[i];
			}
		}
		else if (key == "BUFF") {
			for (size_t i = 0; i < item.buffs.size(); ++i) {
				BuffEntry& entry = item.buffs[i];
				iss >> entry.vnum;
				iss >> entry.effect.min;
				iss >> entry.effect.max;
				iss >> entry.bcard_sub;
				iss >> entry.target;
			}
		}
		else if (key == "LINEDESC") {
			iss >> item.ingame_desc_lines_count;
			iss >> item.description_code_name;
		}
		else if (key == "END") {
			// Add the current item
			items.emplace(item.vnum, item);
		}
	}
}

const nosbazar::io::Item& nosbazar::io::ItemDatParser::item_data(uint32_t vnum) const
{
	return items.at(vnum);
}

nlohmann::json nosbazar::io::Item::json() const {
	using json = nlohmann::json;
	json json_obj;

	json_obj["vnum"] = vnum;
	json_obj["price"] = price;
	json_obj["name"] = LangManager::get_instance().get_all_item_translations(item_name_code);
	json_obj["inventory_tab"] = inventory_tab;
	json_obj["item_type"] = item_type;
	json_obj["item_subtype"] = item_subtype;
	json_obj["equip_slot"] = equip_slot;
	json_obj["icon_id"] = icon_id;
	json_obj["visual_change_id"] = visual_change_id;
	json_obj["attack_type"] = attack_type;
	json_obj["required_class"] = required_class;

	// Flags serialization
	json j_flags;
	//j_flags["unknown_1"] = flags.unknown_1;
	//j_flags["unknown_2"] = flags.unknown_2;
	//j_flags["unknown_3"] = flags.unknown_3;
	j_flags["no_selling"] = flags.no_selling;
	j_flags["no_dropping"] = flags.no_dropping;
	j_flags["no_trading"] = flags.no_trading;
	j_flags["miniland_item"] = flags.miniland_item;
	j_flags["miniland_item_2"] = flags.miniland_item_2;
	j_flags["show_warning_on_use"] = flags.show_warning_on_use;
	j_flags["is_time_space_reward_box"] = flags.is_time_space_reward_box;
	j_flags["show_description_on_hover"] = flags.show_description_on_hover;
	//j_flags["unknown_4"] = flags.unknown_4;
	j_flags["follow_mouse_on_use"] = flags.follow_mouse_on_use;
	j_flags["show_something_on_hover"] = flags.show_something_on_hover;
	j_flags["can_be_colored"] = flags.can_be_colored;
	j_flags["female_can_wear"] = flags.female_can_wear;
	j_flags["male_can_wear"] = flags.male_can_wear;
	//j_flags["unknown_5"] = flags.unknown_5;
	j_flags["play_sound_on_pickup"] = flags.play_sound_on_pickup;
	j_flags["use_reputation_as_price"] = flags.use_reputation_as_price;
	j_flags["is_champion_equip"] = flags.is_champion_equip;
	//j_flags["unknown_6"] = flags.unknown_6;
	j_flags["is_limited"] = flags.is_limited;
	//j_flags["unknown_7"] = flags.unknown_7;
	//j_flags["unknown_8"] = flags.unknown_8;
	json_obj["flags"] = j_flags;

	// Array data
	json j_data = json::array();
	for (uint16_t d : data) {
		j_data.push_back(d);
	}
	json_obj["data"] = j_data;

	// Buffs serialization
	json j_buffs = json::array();
	for (const auto& buff : buffs) {
		json b;
		b["vnum"] = buff.vnum;
		b["effect"] = json{ {"min", buff.effect.min}, {"max", buff.effect.max} };
		b["bcard_sub"] = buff.bcard_sub;
		b["target"] = buff.target;
		j_buffs.push_back(b);
	}
	json_obj["buffs"] = j_buffs;

	// Metadata
	//json_obj["ingame_desc_lines_count"] = ingame_desc_lines_count;
	json_obj["description"] = LangManager::get_instance().get_all_item_translations(description_code_name);

	return json_obj;
}
