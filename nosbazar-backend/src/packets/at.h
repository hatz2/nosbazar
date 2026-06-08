#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>

namespace nosbazar::packets {
	struct At {
		static inline std::string_view opcode = "at";

		explicit At(std::string_view packet) {
			std::string_view header = strings::token<std::string_view>(packet);
		}
	};
}