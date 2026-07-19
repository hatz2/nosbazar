#include "monster_dat_parser.h"
#include "lang_manager.h"
#include <sstream>

namespace nosbazar::io {

    MonsterDatParser& MonsterDatParser::instance()
    {
        static MonsterDatParser instance;
        return instance;
    }

    void MonsterDatParser::parse(const std::string& file_content)
    {
        std::istringstream iss(file_content);
        std::string key;
        Monster monster;
        bool has_vnum = false;

        while (iss >> key) {
            if (key.empty() || key[0] == '#') {
                continue;
            }
            if (key == "VNUM") {
                if (has_vnum) {
                    monsters.emplace(monster.vnum, std::move(monster));
                }
                monster = Monster{};
                iss >> monster.vnum;
                has_vnum = true;
            }
            else if (key == "NAME") {
                iss >> monster.name;
            }
            else if (key == "SETTING") {
                iss >> monster.icon_id;
                for (int i = 0; i < 5; ++i) {
                    int discard;
                    iss >> discard;
                }
            }
        }
        if (has_vnum) {
            monsters.emplace(monster.vnum, std::move(monster));
        }
    }

    const Monster& MonsterDatParser::monster_data(uint32_t vnum) const
    {
        return monsters.at(vnum);
    }

    bool MonsterDatParser::has_monster(uint32_t vnum) const
    {
        return monsters.contains(vnum);
    }

    nlohmann::json Monster::json() const
    {
        nlohmann::json j;
        j["vnum"] = vnum;
        j["name"] = LangManager::get_instance().get_all_monster_translations(name);
        j["icon_id"] = icon_id + 8000;
        return j;
    }
}
