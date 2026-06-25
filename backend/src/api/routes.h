#pragma once

#include <crow.h>

namespace nosbazar::api {

crow::response handle_search(const crow::request& req);
crow::response handle_item_static(const crow::request& req, uint32_t vnum);
crow::response handle_icon(const crow::request& req, uint32_t icon_id);
crow::response handle_servers(const crow::request& req);

} // namespace nosbazar::api
