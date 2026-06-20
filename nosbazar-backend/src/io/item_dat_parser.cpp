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
