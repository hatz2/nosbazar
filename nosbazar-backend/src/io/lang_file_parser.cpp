#include "lang_file_parser.h"
#include <spdlog/spdlog.h>
#include <strings/parse.h>
#include <regex>

namespace nosbazar::io {
	LangFileParser::LangFileParser(const std::string& file_content)
	{
		std::istringstream iss(file_content);
		std::string line;

		while (std::getline(iss, line, '\r')) {
			std::string_view line_view(line);
			std::string key = strings::token<std::string>(line_view, '\t');
			std::string value = strings::token<std::string>(line_view, '\r');
			translations.emplace(std::move(key), std::move(value));
		}
	}
	std::string LangFileParser::translation(const std::string& code_name)
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

