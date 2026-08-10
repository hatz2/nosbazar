#include "api/routes.h"
#include "server_registry.h"
#include <bazar_search_queue.h>
#include <packets/c_blist.h>
#include <io/item_dat_parser.h>
#include <io/bcard_parser.h>
#include <io/monster_dat_parser.h>
#include <io/skill_dat_parser.h>
#include <io/nsip_data_reader.h>
#include <io/const_string_parser.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <memory>
#include <string>
#include <vector>

namespace nosbazar::api {

crow::response handle_search(const crow::request& req)
{
    SPDLOG_DEBUG(req.body);

    crow::json::rvalue json = crow::json::load(req.body);

    int64_t server = json["server"].i();

    auto task = std::make_shared<nosbazar::BazarSearch>();

    nosbazar::packets::CBlist search_packet{
        .index = (int)json["filters"]["index"].i(),
        .type_filter = (int)json["filters"]["category"].i(),
        .sub_type_filter = (int)json["filters"]["sub_category"].i(),
        .level_filter = (int)json["filters"]["level"].i(),
        .rare_filter = (int)json["filters"]["rare"].i(),
        .upgrade_filter = (int)json["filters"]["upgrade"].i(),
        .order_filter = (int)json["filters"]["order"].i()
    };

    for (auto& vnum : json["filters"]["vnum"].lo()) {
        search_packet.vnums_filter.push_back(vnum.i());
    }

    task->request.search_packet = std::move(search_packet);

    nosbazar::BazarSearchManager::queue(server).push(task);

    std::string result = task->response.packet.get_future().get();

    return crow::response(result);
}

crow::response handle_item_static(const crow::request& req, uint32_t vnum)
{
    try {
        const nosbazar::io::Item& item = nosbazar::io::ItemDatParser::instance().item_data(vnum);

        nlohmann::json json_data = item.json();
        std::string result = json_data.dump(2);

        return crow::response(result);
    } catch (const std::out_of_range& e) {
        SPDLOG_WARN("Item not found for vnum: {}", vnum);
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
        SPDLOG_WARN("Monster not found for vnum: {}", vnum);
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
		SPDLOG_WARN("Skill not found for vnum: {}", vnum);
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
        SPDLOG_WARN("BCard not found for vnum: {} sub: {}", vnum, bcard_sub);
        return crow::response(404, "BCard data not found.");
    } catch (const std::exception& e) {
        SPDLOG_ERROR("Internal error formatting bcard: {}", e.what());
        return crow::response(500, "Internal Server Error.");
    }
}

} // namespace nosbazar::api
