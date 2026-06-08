#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>

namespace nosbazar::packets {
	struct In {
		static inline std::string_view opcode = "in";

		game::EntityType entity_type;
		int entity_id{};
		std::optional<std::string> name; // only if entity is player
		std::optional<std::string> family; // only if entity is player
		std::optional<int> vnum; // only if entity is not player
		int x{};
		int y{};

		explicit In(std::string_view packet) {
			std::string_view header = strings::token<std::string_view>(packet);
			
			entity_type = static_cast<game::EntityType>(strings::token<int>(packet));

			if (entity_type == game::EntityType::player) {
				name = strings::token<std::string>(packet);
				family = strings::token<std::string>(packet);
				entity_id = strings::token<int>(packet);
			}
			else {
				vnum = strings::token<int>(packet);
				entity_id = strings::token<int>(packet);
			}

			x = strings::token<int>(packet);
			y = strings::token<int>(packet);
		}
	};
}