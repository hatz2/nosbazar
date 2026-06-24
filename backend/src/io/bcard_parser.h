#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "lang_manager.h"

namespace nosbazar::io {

    constexpr size_t BCARD_MAX_RECORDS = 5;

    struct BCardRecord {
        uint8_t desc_type{};
        std::string subject_code_name{};
        std::string list_code_name_positive{};
        std::string list_code_name_negative{};
    };

    struct BCardEntry {
        uint32_t vnum{};
        int32_t icon{};
        std::string name_code_name{};
        std::array<BCardRecord, BCARD_MAX_RECORDS> records{};

        nlohmann::json json() const;
    };

    class BCardParser {
    public:
        static BCardParser& instance();

        void parse(const std::string& file_content);

        const BCardEntry& bcard_data(uint32_t vnum) const;
        bool has_bcard(uint32_t vnum) const;

        nlohmann::json format_bcard_string(uint32_t bcard_vnum, uint32_t bcard_sub,
                                            int32_t effect_val_1, int32_t effect_val_2) const;

    private:
        BCardParser() = default;
        BCardParser(BCardParser&) = delete;
        void operator=(BCardParser&) = delete;

        std::unordered_map<uint32_t, BCardEntry> bcards;
    };

}
