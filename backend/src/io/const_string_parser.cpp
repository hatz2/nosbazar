#include "const_string_parser.h"
#include "nos_file_text_reader.h"
#include <crypto/hash.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <strings/encoding.h>
#include <strings/parse.h>

namespace nosbazar::io {

void ConstStringParser::load_language(Language lang, const std::string& lang_code)
{
	std::string filepath = fmt::format("assets/NostaleData/NScliData_{}.NOS", lang_code);

	NosFileTextReader reader(filepath);

	if (reader.get_filenames().empty()) {
		SPDLOG_WARN("NScliData_{}.NOS not found or empty", lang_code);
		return;
	}

	std::string content = reader.get_file_content("conststring.dat");
	if (content.empty()) {
		SPDLOG_WARN("conststring.dat not found in NScliData_{}.NOS", lang_code);
		return;
	}

	raw_content.append(content);

	strings::Encoding encoding = encoding_for(lang);
	std::unordered_map<int, std::string> entries;

	std::istringstream iss(content);
	std::string line;
	while (std::getline(iss, line, '\r')) {
		if (line.empty())
			continue;

		std::string_view sv(line);
		int id = strings::token<int>(sv, '\v');

		std::string translation = strings::convert_to_utf8(std::string(sv), encoding);
		entries.emplace(id, std::move(translation));
	}

	size_t count = entries.size();
	translations.emplace(lang, std::move(entries));
	SPDLOG_INFO("Loaded {} const strings for NScliData_{}.NOS", count, lang_code);
}

ConstStringParser::ConstStringParser()
{
	for (const auto& [lang, code] : language_string) {
		load_language(lang, code);
	}

	data_hash = crypto::sha1(raw_content);
	SPDLOG_INFO("Const string data hash: {}", data_hash);
}

const std::string& ConstStringParser::get_data_hash() const
{
	return data_hash;
}

ConstStringParser& ConstStringParser::instance()
{
	static ConstStringParser inst;
	return inst;
}

nlohmann::json ConstStringParser::get_all_translations() const
{
	nlohmann::json result;
	for (const auto& [lang, code] : language_string) {
		if (!translations.contains(lang)) {
			result[code] = nlohmann::json::object();
			continue;
		}
		result[code] = nlohmann::json::object();
		for (const auto& [id, trans] : translations.at(lang)) {
			result[code][std::to_string(id)] = trans;
		}
	}
	return result;
}

nlohmann::json ConstStringParser::get_translations(int id) const
{
	nlohmann::json result;
	for (const auto& [lang, code] : language_string) {
		if (translations.contains(lang) && translations.at(lang).contains(id)) {
			result[code] = translations.at(lang).at(id);
		}
	}
	return result;
}

}
