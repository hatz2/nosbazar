#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>

namespace nosbazar::packets {
	struct Cond {
		static inline std::string_view opcode = "cond";
		game::EntityType entity_type;
		int entity_id;
		int speed;

		explicit Cond(std::string_view packet) {
			std::string_view header = strings::token<std::string_view>(packet);

			entity_type = static_cast<game::EntityType>(strings::token<int>(packet));
			entity_id = strings::token<int>(packet);

			// unknown
			strings::token<std::string_view>(packet);
			strings::token<std::string_view>(packet);

			speed = strings::token<int>(packet);
		}
	};
}