#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace nosbazar::io {

    struct Monster {
        uint32_t vnum{};
        std::string name{};
        uint32_t icon_id{};

        nlohmann::json json() const;
    };

    class MonsterDatParser {
    public:
        static MonsterDatParser& instance();

        void parse(const std::string& file_content);

        const Monster& monster_data(uint32_t vnum) const;
        bool has_monster(uint32_t vnum) const;

    private:
        MonsterDatParser() = default;
        MonsterDatParser(MonsterDatParser&) = delete;
        void operator=(MonsterDatParser&) = delete;

        std::unordered_map<uint32_t, Monster> monsters;
    };
}
