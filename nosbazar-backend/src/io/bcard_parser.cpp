#include "bcard_parser.h"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fmt/format.h>

namespace nosbazar::io {

    BCardParser& BCardParser::instance()
    {
        static BCardParser instance;
        return instance;
    }

    void BCardParser::parse(const std::string& file_content)
    {
        std::istringstream iss(file_content);
        std::string line;
        BCardEntry* current_entry = nullptr;

        while (std::getline(iss, line, '\r')) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream line_iss(line);
            std::string key;
            line_iss >> key;

            if (key == "VNUM") {
                BCardEntry entry;
                line_iss >> entry.vnum;
                auto [it, inserted] = bcards.emplace(entry.vnum, std::move(entry));
                current_entry = &it->second;
            }
            else if (!current_entry) {
                continue;
            }
            else if (key == "ICON") {
                line_iss >> current_entry->icon;
            }
            else if (key == "NAME") {
                line_iss >> current_entry->name_code_name;
            }
            else if (key == "DESC") {
                for (size_t i = 0; i < BCARD_MAX_RECORDS; ++i) {
                    int val;
                    line_iss >> val;
                    current_entry->records[i].desc_type = static_cast<uint8_t>(val);
                }
            }
            else if (key.rfind("SUBJ", 0) == 0) {
                int index = std::stoi(key.substr(4)) - 1;
                if (index >= 0 && index < static_cast<int>(BCARD_MAX_RECORDS)) {
                    line_iss >> current_entry->records[index].subject_code_name;
                }
            }
            else if (key.rfind("LIST", 0) == 0) {
                size_t dash_pos = key.find('-');
                if (dash_pos != std::string::npos) {
                    int index = std::stoi(key.substr(4, dash_pos - 4)) - 1;
                    int variant = std::stoi(key.substr(dash_pos + 1));
                    if (index >= 0 && index < static_cast<int>(BCARD_MAX_RECORDS)) {
                        std::string code_name;
                        line_iss >> code_name;
                        if (variant == 1) {
                            current_entry->records[index].list_code_name_positive = std::move(code_name);
                        } else {
                            current_entry->records[index].list_code_name_negative = std::move(code_name);
                        }
                    }
                }
            }
        }
    }

    const BCardEntry& BCardParser::bcard_data(uint32_t vnum) const
    {
        return bcards.at(vnum);
    }

    bool BCardParser::has_bcard(uint32_t vnum) const
    {
        return bcards.contains(vnum);
    }

    static int32_t floor_div_abs(int32_t value)
    {
        int32_t div = value / 4;
        int32_t rem = value % 4;
        if (rem != 0 && value < 0) {
            --div;
        }
        return std::abs(div);
    }

    static std::string substitute_variables(uint8_t desc_type, const std::string& template_str,
                                             int32_t effect_val_1, int32_t effect_val_2,
                                             uint32_t bcard_vnum)
    {
        std::string result = template_str;

        size_t pos = 0;
        while ((pos = result.find("%%", pos)) != std::string::npos) {
            result.replace(pos, 2, "%");
            pos += 1;
        }

        if (desc_type == 0) {
            return result;
        }

        bool both_mobs = (bcard_vnum == 46);

        int32_t first_val = (desc_type == 5 || both_mobs)
            ? effect_val_1
            : floor_div_abs(effect_val_1);

        pos = result.find("%s");
        if (pos != std::string::npos) {
            result.replace(pos, 2, std::to_string(first_val));
        }

        pos = result.find("%s");
        if (pos == std::string::npos) return result;

        if (desc_type == 1) {
            return result;
        }

        if (desc_type == 2 || desc_type == 5) {
            result.replace(pos, 2, std::to_string(floor_div_abs(effect_val_2)));
        } else {
            result.replace(pos, 2, std::to_string(effect_val_2));
        }

        return result;
    }

    nlohmann::json BCardParser::format_bcard_string(uint32_t bcard_vnum, uint32_t bcard_sub,
                                                     int32_t effect_val_1, int32_t effect_val_2) const
    {
        const BCardEntry& entry = bcards.at(bcard_vnum);
        const BCardRecord& record = entry.records.at(bcard_sub);

        std::string code_name = (effect_val_1 >= 0)
            ? record.list_code_name_positive
            : record.list_code_name_negative;

        auto& lm = LangManager::get_instance();
        nlohmann::json result;

        for (const auto& [lang, code] : language_string) {
            std::string lang_lower = code;
            std::transform(lang_lower.begin(), lang_lower.end(), lang_lower.begin(), ::tolower);
            std::string filename = fmt::format("_code_{}_BCard.txt", lang_lower);

            std::string raw = lm.get_translation(lang, filename, code_name);
            std::string formatted = substitute_variables(
                record.desc_type, raw, effect_val_1, effect_val_2, bcard_vnum);
            result[code] = formatted;
        }

        return result;
    }

    nlohmann::json BCardEntry::json() const {
        nlohmann::json j;
        j["vnum"] = vnum;
        j["icon"] = icon;
        j["name"] = LangManager::get_instance().get_all_translations("_code_{}_BCard.txt", name_code_name);

        nlohmann::json j_records = nlohmann::json::array();
        for (size_t i = 0; i < BCARD_MAX_RECORDS; ++i) {
            const auto& rec = records[i];
            nlohmann::json jr;
            jr["desc_type"] = rec.desc_type;
            jr["subject"] = LangManager::get_instance().get_all_translations("_code_{}_BCard.txt", rec.subject_code_name);
            jr["description_positive"] = LangManager::get_instance().get_all_translations("_code_{}_BCard.txt", rec.list_code_name_positive);
            jr["description_negative"] = LangManager::get_instance().get_all_translations("_code_{}_BCard.txt", rec.list_code_name_negative);
            j_records.push_back(std::move(jr));
        }
        j["records"] = std::move(j_records);

        return j;
    }

}
