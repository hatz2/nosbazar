#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>
#include <fmt/format.h>

namespace nosbazar::packets {
	struct Walk {
		static inline std::string_view opcode = "walk";

		int x{};
		int y{};
		int unknown{};
		int speed{};

		explicit Walk(int x, int y, int unknown, int speed)
			: x(x), y(y), unknown(unknown), speed(speed) {}

		explicit Walk(std::string_view packet) {
			std::string_view header = strings::token<std::string_view>(packet);

			x = strings::token<int>(packet);
			y = strings::token<int>(packet);
			unknown = strings::token<int>(packet);
			speed = strings::token<int>(packet);
		}

		std::string string() const {
			return fmt::format("{} {} {} {} {}", opcode, x, y, unknown, speed);
		}
	};
}