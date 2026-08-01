#pragma once

#include <crow.h>

namespace nosbazar::api {

crow::response handle_search(const crow::request& req);
crow::response handle_item_static(const crow::request& req, uint32_t vnum);
crow::response handle_monster_static(const crow::request& req, uint32_t vnum);
crow::response handle_skill_static(const crow::request& req, uint32_t vnum);
crow::response handle_icon(const crow::request& req, uint32_t icon_id);
crow::response handle_bcard_string(const crow::request& req);
crow::response handle_servers(const crow::request& req);
crow::response handle_const_string(const crow::request& req, uint32_t id);

} // namespace nosbazar::api
