#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>

namespace nosbazar::packets {
	struct CMap {
		static inline std::string_view opcode = "c_map";
		
		int unknown;
		int id;
		bool is_new_map;


		explicit CMap(std::string_view packet) {
			std::string_view header = strings::token<std::string_view>(packet);

			unknown = strings::token<int>(packet);
			id = strings::token<int>(packet);
			is_new_map = strings::token<bool>(packet);
		}
	};
}