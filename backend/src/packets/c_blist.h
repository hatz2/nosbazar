#pragma once

#include <string>
#include <game/enums.h>
#include <optional>
#include <strings/parse.h>
#include <fmt/format.h>
#include <vector>

namespace nosbazar::packets {
	struct CBlist {
		static inline std::string_view opcode = "c_blist";

		int index{};
		int type_filter{};
		int sub_type_filter{};
		int level_filter{};
		int rare_filter{};
		int upgrade_filter{};
		int order_filter{};
		int unknown1{};
		int unknown2{};
		std::vector<uint16_t> vnums_filter;

		std::string string() const {
			std::string packet = fmt::format(
				"{}  {} {} {} {} {} {} {} {} {} {}",
				opcode,
				index,
				type_filter,
				sub_type_filter,
				level_filter,
				rare_filter,
				upgrade_filter,
				order_filter,
				unknown1,
				unknown2,
				vnums_filter.size()
			);

			std::string vnums;
			for (uint16_t vnum : vnums_filter) {
				vnums += fmt::format(" {}", vnum);
			}

			return fmt::format("{}{}", packet, vnums);
		}
	};
}