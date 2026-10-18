#pragma once

#include <bazar_search_queue.h>
#include <io/language.h>
#include <packets/c_blist.h>
#include <cstddef>
#include <expected>
#include <string>
#include <string_view>

namespace nosbazar::api {

inline constexpr std::size_t max_vnum_search = 50;

struct SearchValidationError {
    std::string field;
    std::string message;
};

struct SearchRequest {
    GameServer server{};
    packets::CBlist search_packet;
    io::Language language{io::Language::english};
    std::string name;
};

std::expected<SearchRequest, SearchValidationError> parse_search_request(std::string_view body);

} // namespace nosbazar::api
