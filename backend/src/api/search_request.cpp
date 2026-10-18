#include "api/search_request.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <utility>

namespace {
    using nosbazar::api::SearchValidationError;

    struct CategoryRules {
        int sub_category;
        int level;
        int rare;
        int upgrade;
    };

    // Matches the options in the frontend category filter components.
    constexpr std::array<CategoryRules, 13> category_rules{{
        {0, 0, 0, 0},     // All
        {5, 21, 9, 14},   // Weapon
        {5, 21, 9, 14},   // Armour
        {12, 21, 0, 0},   // Equipment
        {6, 21, 0, 0},    // Accessories
        {59, 10, 10, 21}, // Specialist (subcategory also has gaps)
        {2, 10, 0, 7},    // Pet
        {6, 10, 0, 0},    // Partner
        {2, 10, 9, 0},    // Shell
        {7, 0, 0, 0},     // Main item
        {7, 0, 0, 0},     // Consumer item
        {0, 0, 0, 0},     // Miscellaneous
        {2, 0, 0, 0}      // Store mount
    }};

    constexpr std::array specialist_subcategories{
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
        32, 33, 34, 35, 40, 41, 42, 43, 46, 47, 48, 49,
        52, 53, 54, 55, 56, 57, 58, 59
    };

    std::expected<int, SearchValidationError> read_integer(
        const nlohmann::json& value, std::string field, int maximum)
    {
        if (!value.is_number_integer()) {
            return std::unexpected(SearchValidationError{
                std::move(field), "Expected an integer."});
        }

        // Check unsigned JSON integers before reading them as a signed type.
        if (value.is_number_unsigned()) {
            const auto number = value.get<std::uint64_t>();
            if (number <= static_cast<std::uint64_t>(maximum)) {
                return static_cast<int>(number);
            }
        } else {
            const auto number = value.get<std::int64_t>();
            if (number >= 0 && number <= maximum) {
                return static_cast<int>(number);
            }
        }

        return std::unexpected(SearchValidationError{
            std::move(field), "Expected a value between 0 and " + std::to_string(maximum) + "."});
    }

    std::expected<int, SearchValidationError> read_field(
        const nlohmann::json& object, const char* key, std::string field, int maximum)
    {
        const auto it = object.find(key);
        if (it == object.end()) {
            // Empty frontend selectors can omit category-inapplicable filters.
            if (maximum == 0) {
                return 0;
            }
            return std::unexpected(SearchValidationError{
                std::move(field), "Missing required field."});
        }
        return read_integer(*it, std::move(field), maximum);
    }
}

namespace nosbazar::api {

std::expected<SearchRequest, SearchValidationError> parse_search_request(std::string_view body)
{
    const auto json = nlohmann::json::parse(body.begin(), body.end(), nullptr, false);
    if (json.is_discarded() || !json.is_object()) {
        return std::unexpected(SearchValidationError{"body", "Expected a valid JSON object."});
    }

    SearchRequest request;
    const auto server = read_field(json, "server", "server", std::numeric_limits<GameServer>::max());
    if (!server) {
        return std::unexpected(server.error());
    }
    request.server = *server;

    const auto filters_it = json.find("filters");
    if (filters_it == json.end() || !filters_it->is_object()) {
        return std::unexpected(SearchValidationError{"filters", "Expected a filters object."});
    }
    const auto& filters = *filters_it;
    const auto category = read_field(filters, "category", "filters.category", 12);
    if (!category) {
        return std::unexpected(category.error());
    }
    request.search_packet.type_filter = *category;
    const auto& rules = category_rules[static_cast<std::size_t>(*category)];

    struct FilterField {
        const char* key;
        int maximum;
        int packets::CBlist::* member;
    };
    const std::array fields{
        FilterField{"index", std::numeric_limits<int>::max(), &packets::CBlist::index},
        FilterField{"sub_category", rules.sub_category, &packets::CBlist::sub_type_filter},
        FilterField{"level", rules.level, &packets::CBlist::level_filter},
        FilterField{"rare", rules.rare, &packets::CBlist::rare_filter},
        FilterField{"upgrade", rules.upgrade, &packets::CBlist::upgrade_filter},
        FilterField{"order", 3, &packets::CBlist::order_filter}
    };
    for (const auto& field : fields) {
        const auto value = read_field(filters, field.key, std::string("filters.") + field.key, field.maximum);
        if (!value) {
            return std::unexpected(value.error());
        }
        request.search_packet.*(field.member) = *value;
    }

    if (*category == 5 && std::find(specialist_subcategories.begin(), specialist_subcategories.end(),
            request.search_packet.sub_type_filter) == specialist_subcategories.end()) {
        return std::unexpected(SearchValidationError{
            "filters.sub_category", "Invalid subcategory for the selected category."});
    }

    const auto vnums_it = filters.find("vnum");
    if (vnums_it == filters.end() || !vnums_it->is_array()) {
        return std::unexpected(SearchValidationError{"filters.vnum", "Expected an array of item IDs."});
    }
    if (vnums_it->size() > max_vnum_search) {
        return std::unexpected(SearchValidationError{"filters.vnum", "A maximum of 50 item IDs is allowed."});
    }
    for (std::size_t index = 0; index < vnums_it->size(); ++index) {
        const auto vnum = read_integer((*vnums_it)[index], "filters.vnum[" + std::to_string(index) + "]",
            std::numeric_limits<std::uint16_t>::max());
        if (!vnum) {
            return std::unexpected(vnum.error());
        }
        request.search_packet.vnums_filter.push_back(static_cast<std::uint16_t>(*vnum));
    }

    if (const auto it = json.find("name"); it != json.end()) {
        if (!it->is_string()) {
            return std::unexpected(SearchValidationError{"name", "Expected a string."});
        }
        request.name = it->get<std::string>();
        const auto first = request.name.find_first_not_of(" \t\r\n\f\v");
        if (first == std::string::npos) {
            request.name.clear();
        } else {
            const auto last = request.name.find_last_not_of(" \t\r\n\f\v");
            request.name = request.name.substr(first, last - first + 1);
        }
    }

    if (const auto it = json.find("lang"); it != json.end()) {
        if (!it->is_string()) {
            return std::unexpected(SearchValidationError{"lang", "Expected a language code string."});
        }
        const auto language = io::string_to_lang.find(it->get<std::string>());
        if (language == io::string_to_lang.end()) {
            return std::unexpected(SearchValidationError{"lang", "Unsupported language code."});
        }
        request.language = language->second;
    }

    return request;
}

} // namespace nosbazar::api
