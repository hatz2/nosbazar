#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <unordered_map>
#include <variant>
#include <sstream>

namespace nosbazar::io {
	class LangFileParser {
	public:
		LangFileParser(const std::string& file_content);

		std::string translation(const std::string& code_name);

	private:
		std::unordered_map<std::string, std::string> translations;
	};
}
