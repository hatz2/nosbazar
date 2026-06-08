#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>

namespace nosbazar::packets {
	struct Gp {
		static inline std::string_view opcode = "gp";

		int x;
		int y;
		int id;


		explicit Gp(std::string_view packet) {
			std::string_view header = strings::token<std::string_view>(packet);

			x = strings::token<int>(packet);
			y = strings::token<int>(packet);
			id = strings::token<int>(packet);
		}
	};
}