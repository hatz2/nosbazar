#include "lang_file_parser.h"
#include <spdlog/spdlog.h>
#include <strings/parse.h>

namespace {

void replace_newline_tags(std::string& text)
{
	size_t pos = 0;
	while ((pos = text.find("[n]", pos)) != std::string::npos) {
		text.replace(pos, 3, "\n");
		pos += 1;
	}
}

}

namespace nosbazar::io {
	LangFileParser::LangFileParser(const std::string& file_content, strings::Encoding encoding)
	{
		std::istringstream iss(file_content);
		std::string line;

		while (std::getline(iss, line, '\r')) {
			std::string_view line_view(line);
			std::string key = strings::token<std::string>(line_view, '\t');
		std::string value = strings::token<std::string>(line_view, '\r');
			value = strings::convert_to_utf8(value, encoding);
			replace_newline_tags(value);
			translations.emplace(std::move(key), std::move(value));
		}
	}

	const std::string& LangFileParser::translation(const std::string& code_name) const
	{
		if (translations.contains(code_name)) {
			return translations.at(code_name);
		}
		else {
		SPDLOG_ERROR("Could not find translation for key: {}", code_name);
			return code_name;
		}
	}
}
