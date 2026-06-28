#pragma once

#include "language.h"
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace nosbazar::io {

class ConstStringParser {
public:
	static ConstStringParser& instance();

	nlohmann::json get_all_translations() const;

private:
	ConstStringParser();
	ConstStringParser(const ConstStringParser&) = delete;
	void operator=(const ConstStringParser&) = delete;

	void load_language(Language lang, const std::string& lang_code);

	std::unordered_map<Language, std::unordered_map<int, std::string>> translations;
};

}
