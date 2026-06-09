#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>

namespace nosbazar::packets {
	struct At {
		static inline std::string_view opcode = "at";

		int character_id;
		int map_id;
		int x;
		int y;

		explicit At(std::string_view packet) {
			std::string_view header = strings::token<std::string_view>(packet);
			character_id = strings::token<int>(packet);
			map_id = strings::token<int>(packet);
			x = strings::token<int>(packet);
			y = strings::token<int>(packet);
		}
	};
}