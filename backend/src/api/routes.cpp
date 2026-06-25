#include "api/routes.h"
#include "server_registry.h"
#include <bazar_search_queue.h>
#include <packets/c_blist.h>
#include <io/item_dat_parser.h>
#include <io/nsip_data_reader.h>
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

} // namespace nosbazar::api
