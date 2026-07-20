#include "skill_dat_parser.h"
#include "lang_manager.h"
#include <sstream>

namespace nosbazar::io {

SkillDatParser& SkillDatParser::instance()
{
	static SkillDatParser instance;
	return instance;
}

void SkillDatParser::parse(const std::string& file_content)
{
	std::istringstream iss(file_content);
	std::string key;
	Skill skill;
	bool has_vnum = false;

	while (iss >> key) {
		if (key.empty() || key[0] == '#') {
			continue;
		}
		if (key == "VNUM") {
			if (has_vnum) {
				skills.emplace(skill.vnum, std::move(skill));
			}
			skill = Skill{};
			iss >> skill.vnum;
			has_vnum = true;
		} else if (key == "NAME") {
			iss >> skill.name;
		}
	}
	if (has_vnum) {
		skills.emplace(skill.vnum, std::move(skill));
	}
}

const Skill& SkillDatParser::skill_data(uint32_t vnum) const
{
	return skills.at(vnum);
}

bool SkillDatParser::has_skill(uint32_t vnum) const
{
	return skills.contains(vnum);
}

nlohmann::json Skill::json() const
{
	nlohmann::json j;
	j["vnum"] = vnum;
	j["name"] = LangManager::get_instance().get_all_skill_translations(name);
	return j;
}

}
