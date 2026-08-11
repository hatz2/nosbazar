#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <array>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <variant>
#include <sstream>
#include <nlohmann/json.hpp>
#include "lang_manager.h"

namespace nosbazar::io {

    struct ItemFlags {
        uint32_t unknown_1{};
        uint32_t unknown_2{};
        uint32_t unknown_3{};
        bool no_selling{};
        bool no_dropping{};
        bool no_trading{};
        bool miniland_item{};
        bool miniland_item_2{};
        bool show_warning_on_use{};
        bool is_time_space_reward_box{};
        bool show_description_on_hover{};
        uint32_t unknown_4{};
        bool follow_mouse_on_use{};
        bool show_something_on_hover{};
        bool can_be_colored{};
        bool female_can_wear{};
        bool male_can_wear{};
        uint32_t unknown_5{};
        bool play_sound_on_pickup{};
        bool use_reputation_as_price{};
        bool is_champion_equip{};
        uint32_t unknown_6{};
        bool is_limited{};
        uint32_t unknown_7{};
        uint32_t unknown_8{};
    };

    struct BuffEntry {
        int32_t vnum{};
        int32_t val_1{};
        int32_t val_2{};
        uint32_t bcard_sub{};
        uint32_t target{};
    };


    // TODO: Parse actual DATA types into the std::variant
    struct Item {
        uint32_t vnum{};
        uint32_t price{};
        std::string item_name_code{};
        uint16_t inventory_tab{};
        uint16_t item_type{};
        uint16_t item_subtype{};
        uint16_t equip_slot{};
        uint32_t icon_id{};
        uint32_t visual_change_id{};
        uint16_t attack_type{};
        uint16_t required_class{};
        ItemFlags flags{};
        std::array<uint16_t, 20> data{};
        std::array<BuffEntry, 5> buffs{};
        uint32_t ingame_desc_lines_count{};
        std::string description_code_name{};

        nlohmann::json json() const;
    };

    class ItemDatParser {
    public:
        static ItemDatParser& instance();

        struct ItemNameMatch {
            uint32_t vnum{};
            std::string name;
            uint32_t icon_id{};
        };

        void parse(const std::string& file_content);

        const Item& item_data(uint32_t vnum) const;

        std::vector<ItemNameMatch> search_by_name(Language lang, std::string_view query, size_t limit = 10) const;

    private:
        struct ItemNameIndexEntry {
            uint32_t vnum{};
            std::string name;
            std::string lower_name;
            uint32_t icon_id{};
        };

        ItemDatParser() = default;
        ItemDatParser(ItemDatParser&) = delete;
        void operator=(ItemDatParser&) = delete;

        const std::vector<ItemNameIndexEntry>& ensure_name_index(Language lang) const;

        std::unordered_map<uint32_t, Item> items;
        mutable std::mutex name_index_mutex;
        mutable std::unordered_map<Language, std::vector<ItemNameIndexEntry>> name_indexes;
    };
}