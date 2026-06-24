#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <unordered_map>
#include <variant>
#include <sstream>
#include <strings/encoding.h>

namespace nosbazar::io {
	class LangFileParser {
	public:
		LangFileParser(const std::string& file_content, strings::Encoding encoding);

		const std::string& translation(const std::string& code_name) const;

	private:
		std::unordered_map<std::string, std::string> translations;
	};
}
