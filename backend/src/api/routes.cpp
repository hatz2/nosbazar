#include "api/routes.h"
#include "api/search_request.h"
#include "server_registry.h"
#include <bazar_search_queue.h>
#include <io/item_dat_parser.h>
#include <io/bcard_parser.h>
#include <io/monster_dat_parser.h>
#include <io/skill_dat_parser.h>
#include <io/nsip_data_reader.h>
#include <io/const_string_parser.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <io/language.h>

namespace {
    crow::response validation_error(const nosbazar::api::SearchValidationError& error)
    {
        nlohmann::json body = {{"error", {
            {"code", "invalid_request"}, {"field", error.field}, {"message", error.message}
        }}};
        crow::response response(400, body.dump());
        response.set_header("Content-Type", "application/json");
        return response;
    }

    crow::response search_error(nosbazar::BazarSearchError error)
    {
        int status = 503;
        const char* code = "server_unavailable";
        const char* message = "No search worker is available for this server.";
        switch (error) {
        case nosbazar::BazarSearchError::unavailable:
            break;
        case nosbazar::BazarSearchError::disconnected:
            code = "disconnected";
            message = "The game connection was lost.";
            break;
        case nosbazar::BazarSearchError::invalid_response:
            status = 502;
            code = "invalid_response";
            message = "The game response could not be decoded.";
            break;
        }
        nlohmann::json body = {{"error", {{"code", code}, {"message", message}}}};
        crow::response response(status, body.dump());
        response.set_header("Content-Type", "application/json");
        return response;
    }
}

namespace nosbazar::api {

crow::response handle_search(const crow::request& req)
{
    SPDLOG_TRACE("Search request body: {}", req.body);

    auto request = parse_search_request(req.body);
    if (!request) {
        return validation_error(request.error());
    }
    if (!ServerRegistry::instance().is_available(request->server)) {
        return search_error(BazarSearchError::unavailable);
    }

    auto& search_packet = request->search_packet;
    if (!request->name.empty()) {
        const auto matches = io::ItemDatParser::instance().search_by_name(
            request->language, request->name, max_vnum_search - search_packet.vnums_filter.size() + 1);

        if (matches.empty()) {
            SPDLOG_INFO("Name filter '{}' matched no items", request->name);
            return crow::response(R"({"items": []})");
        }
        if (search_packet.vnums_filter.size() + matches.size() > max_vnum_search) {
            return validation_error({"name", "The name and item ID filters exceed the maximum of 50 items."});
        }
        for (const auto& match : matches) {
            if (match.vnum > std::numeric_limits<std::uint16_t>::max()) {
                return validation_error({"name", "The name matched an item ID outside the supported range."});
            }
            search_packet.vnums_filter.push_back(static_cast<std::uint16_t>(match.vnum));
        }
        SPDLOG_INFO("Name filter '{}' resolved to {} vnums", request->name, matches.size());
    }

    auto task = std::make_shared<nosbazar::BazarSearch>();
    task->request.search_packet = std::move(search_packet);

    auto response_future = task->get_future();
    if (!nosbazar::BazarSearchManager::submit(request->server, task)) {
        task->fail(BazarSearchError::unavailable);
    }

    auto result = response_future.get();
    if (!result) {
        return search_error(result.error());
    }

    return crow::response(std::move(*result));
}

crow::response handle_item_name_search(const crow::request& req)
{
    const char* query_str = req.url_params.get("q");
    if (query_str == nullptr || *query_str == '\0') {
        return crow::response(400, "Missing required query param: q.");
    }

    io::Language lang = io::Language::english;
    const char* lang_str = req.url_params.get("lang");
    if (lang_str != nullptr) {
        const auto lang_it = io::string_to_lang.find(lang_str);
        if (lang_it != io::string_to_lang.end()) {
            lang = lang_it->second;
        }
    }

    size_t limit = 10;
    const char* limit_str = req.url_params.get("limit");
    if (limit_str != nullptr) {
        try {
            limit = std::stoul(limit_str);
        } catch (const std::exception& e) {
            return crow::response(400, "Invalid limit: must be a positive integer.");
        }
    }
    limit = std::clamp(limit, size_t{1}, size_t{50});

    std::vector<nosbazar::io::ItemDatParser::ItemNameMatch> matches =
        nosbazar::io::ItemDatParser::instance().search_by_name(lang, query_str, limit);

    nlohmann::json json_data = nlohmann::json::array();
    for (const auto& match : matches) {
        json_data.push_back({{"vnum", match.vnum}, {"name", match.name}, {"icon_id", match.icon_id}});
    }

    return crow::response(nlohmann::json{{"matches", json_data}}.dump());
}

crow::response handle_item_static(const crow::request& req, uint32_t vnum)
{
    try {
        const nosbazar::io::Item& item = nosbazar::io::ItemDatParser::instance().item_data(vnum);

        nlohmann::json json_data = item.json();
        std::string result = json_data.dump(2);

        return crow::response(result);
    } catch (const std::out_of_range& e) {
        SPDLOG_INFO("Item not found for vnum: {}", vnum);
        return crow::response(404, "Item data not found.");
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Internal error fetching item data for vnum {} : {}", vnum, e.what());
        return crow::response(500, "Internal Server Error.");
    }
}

crow::response handle_monster_static(const crow::request& req, uint32_t vnum)
{
    try {
        const nosbazar::io::Monster& monster = nosbazar::io::MonsterDatParser::instance().monster_data(vnum);
        nlohmann::json json_data = monster.json();
        return crow::response(json_data.dump(2));
    } catch (const std::out_of_range& e) {
        SPDLOG_INFO("Monster not found for vnum: {}", vnum);
        return crow::response(404, "Monster data not found.");
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Internal error fetching monster data for vnum {} : {}", vnum, e.what());
        return crow::response(500, "Internal Server Error.");
    }
}

crow::response handle_skill_static(const crow::request& req, uint32_t vnum)
{
	try {
		const nosbazar::io::Skill& skill = nosbazar::io::SkillDatParser::instance().skill_data(vnum);
		nlohmann::json json_data = skill.json();
		return crow::response(json_data.dump(2));
	} catch (const std::out_of_range& e) {
		SPDLOG_INFO("Skill not found for vnum: {}", vnum);
		return crow::response(404, "Skill data not found.");
	} catch (const std::exception& e) {
		SPDLOG_ERROR("Internal error fetching skill data for vnum {} : {}", vnum, e.what());
		return crow::response(500, "Internal Server Error.");
	}
}

crow::response handle_icon(const crow::request& req, uint32_t icon_id)
{
    const std::vector<uint8_t>* data = nosbazar::io::NSipDataReader::instance().get_icon_data(icon_id);

    if (!data) {
        data = nosbazar::io::NSipDataReader::instance().get_icon_data(0);
    }

    crow::response response;
    response.set_header("Content-Type", "image/png");

    if (data) {
        response.body.assign(reinterpret_cast<const char*>(data->data()), data->size());
    } else {
        response.code = 404;
        response.body = "Icon not found";
    }

    return response;
}

crow::response handle_servers(const crow::request& req)
{
    auto servers = nosbazar::ServerRegistry::instance().get_servers();

    nlohmann::json json_data = nlohmann::json::array();
    for (const auto& s : servers) {
        json_data.push_back({{"id", s.id}, {"name", s.name}});
    }

    return crow::response(json_data.dump());
}

crow::response handle_const_string(const crow::request& req, uint32_t id)
{
	nlohmann::json json_data = nosbazar::io::ConstStringParser::instance().get_translations(static_cast<int>(id));
	if (json_data.empty()) {
		return crow::response(404, "Const string not found.");
	}
	return crow::response(json_data.dump());
}

crow::response handle_const_string_version(const crow::request& req)
{
	nlohmann::json json_data = {
		{"version", nosbazar::io::ConstStringParser::instance().get_data_hash()}
	};
	return crow::response(json_data.dump());
}

crow::response handle_bcard_string(const crow::request& req)
{
    const char* vnum_str = req.url_params.get("vnum");
    const char* sub_str = req.url_params.get("sub");
    const char* val1_str = req.url_params.get("val1");
    const char* val2_str = req.url_params.get("val2");

    if (!vnum_str || !sub_str) {
        return crow::response(400, "Missing required query params: vnum, sub.");
    }

    uint32_t vnum{};
    uint32_t bcard_sub{};
    int32_t val_1{};
    int32_t val_2{};

    try {
        vnum = std::stoul(vnum_str);
        bcard_sub = std::stoul(sub_str);
        val_1 = val1_str ? std::stoi(val1_str) : 0;
        val_2 = val2_str ? std::stoi(val2_str) : 0;
    } catch (const std::exception& e) {
        return crow::response(400, "Invalid query parameter value.");
    }

    try {
        nlohmann::json json_data = nosbazar::io::BCardParser::instance().format_bcard_string(vnum, bcard_sub, val_1, val_2);
        return crow::response(json_data.dump(2));
    } catch (const std::out_of_range& e) {
        SPDLOG_INFO("BCard not found for vnum: {} sub: {}", vnum, bcard_sub);
        return crow::response(404, "BCard data not found.");
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Internal error formatting bcard: {}", e.what());
        return crow::response(500, "Internal Server Error.");
    }
}

} // namespace nosbazar::api
