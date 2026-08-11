#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <array>
#include <unordered_map>
#include <variant>
#include <optional>
#include <sstream>
#include <strings/encoding.h>

namespace nosbazar::io {
	class LangFileParser {
	public:
		LangFileParser(const std::string& file_content, strings::Encoding encoding);

		const std::string& translation(const std::string& code_name) const;

		std::optional<std::string_view> translation_if_present(const std::string& code_name) const;

	private:
		std::unordered_map<std::string, std::string> translations;
	};
}
