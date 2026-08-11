#include "lang_manager.h"
#include <spdlog/spdlog.h>
#include <algorithm>

namespace nosbazar::io {
	LangManager::LangFile::LangFile(Language lang)
	{
		std::string language = language_string.at(lang);
		std::string filepath = fmt::format("assets/NostaleData/NSlangData_{}.NOS", language);

		NosFileTextReader reader(filepath);

		std::vector<std::string> filenames = reader.get_filenames();

		strings::Encoding encoding = encoding_for(lang);

		for (const std::string& filename : filenames) {
			std::string content = reader.get_file_content(filename);
			auto parser = std::make_unique<LangFileParser>(content, encoding);
			file_parsers.emplace(filename, std::move(parser));
		}
	}
	std::string LangManager::LangFile::translation(const std::string& filename, const std::string& code_name) const
	{
		if (file_parsers.contains(filename)) {
			auto& parser = file_parsers.at(filename);
			return parser->translation(code_name);
		}
		
		return code_name;
	}

	std::optional<std::string_view> LangManager::LangFile::translation_if_present(const std::string& filename, const std::string& code_name) const
	{
		const auto iterator = file_parsers.find(filename);
		if (iterator == file_parsers.end()) {
			return std::nullopt;
		}
		return iterator->second->translation_if_present(code_name);
	}

	LangManager& LangManager::get_instance()
	{
		static LangManager instance;
		return instance;
	}

	std::string LangManager::get_translation(Language lang, const std::string& filename, const std::string& code_name) const
	{
		return lang_files.at(lang)->translation(filename, code_name);
	}

	nlohmann::json LangManager::get_all_translations(const std::string& filename_template, const std::string& code_name) const
	{
		nlohmann::json result;
		for (const auto& [lang, code] : language_string) {
			std::string lang_lower = code;
			std::transform(lang_lower.begin(), lang_lower.end(), lang_lower.begin(), ::tolower);
			std::string filename = fmt::format(fmt::runtime(filename_template), lang_lower);
			result[code] = get_translation(lang, filename, code_name);
		}
		return result;
	}

	std::string LangManager::get_item_translation(Language lang, const std::string& code_name) const
	{
		std::string language = language_string.at(lang);
		std::transform(language.begin(), language.end(), language.begin(), ::tolower);
		std::string filename = fmt::format("_code_{}_Item.txt", language);
		return get_translation(lang, filename, code_name);
	}

	std::string LangManager::get_item_translation_quiet(Language lang, const std::string& code_name) const
	{
		std::string language = language_string.at(lang);
		std::transform(language.begin(), language.end(), language.begin(), ::tolower);
		std::string filename = fmt::format("_code_{}_Item.txt", language);
		const auto value = lang_files.at(lang)->translation_if_present(filename, code_name);
		return value.has_value() ? std::string(value.value()) : std::string{};
	}

	nlohmann::json LangManager::get_all_item_translations(const std::string& code_name) const
	{
		return get_all_translations("_code_{}_Item.txt", code_name);
	}

	std::string LangManager::get_bcard_translation(Language lang, const std::string& code_name) const
	{
		std::string lang_lower = language_string.at(lang);
		std::transform(lang_lower.begin(), lang_lower.end(), lang_lower.begin(), ::tolower);
		std::string filename = fmt::format("_code_{}_BCard.txt", lang_lower);
		return get_translation(lang, filename, code_name);
	}

	nlohmann::json LangManager::get_all_bcard_translations(const std::string& code_name) const
	{
		return get_all_translations("_code_{}_BCard.txt", code_name);
	}

	std::string LangManager::get_monster_translation(Language lang, const std::string& code_name) const
	{
		std::string lang_lower = language_string.at(lang);
		std::transform(lang_lower.begin(), lang_lower.end(), lang_lower.begin(), ::tolower);
		std::string filename = fmt::format("_code_{}_monster.txt", lang_lower);
		std::string result = get_translation(lang, filename, code_name);
		std::replace(result.begin(), result.end(), '^', ' ');
		return result;
	}

	nlohmann::json LangManager::get_all_monster_translations(const std::string& code_name) const
	{
		nlohmann::json result;
		for (const auto& [lang, code] : language_string) {
			result[code] = get_monster_translation(lang, code_name);
		}
		return result;
	}

	std::string LangManager::get_skill_translation(Language lang, const std::string& code_name) const
	{
		std::string lang_lower = language_string.at(lang);
		std::transform(lang_lower.begin(), lang_lower.end(), lang_lower.begin(), ::tolower);
		std::string filename = fmt::format("_code_{}_Skill.txt", lang_lower);
		return get_translation(lang, filename, code_name);
	}

	nlohmann::json LangManager::get_all_skill_translations(const std::string& code_name) const
	{
		return get_all_translations("_code_{}_Skill.txt", code_name);
	}

	LangManager::LangManager()
	{
		lang_files.emplace(Language::spanish, std::make_unique<LangFile>(Language::spanish));
		lang_files.emplace(Language::english, std::make_unique<LangFile>(Language::english));
		lang_files.emplace(Language::french, std::make_unique<LangFile>(Language::french));
		lang_files.emplace(Language::german, std::make_unique<LangFile>(Language::german));
		lang_files.emplace(Language::turkish, std::make_unique<LangFile>(Language::turkish));
		lang_files.emplace(Language::italian, std::make_unique<LangFile>(Language::italian));
		//lang_files.emplace(Language::russian, std::make_unique<LangFile>(Language::russian));
		lang_files.emplace(Language::polish, std::make_unique<LangFile>(Language::polish));
		lang_files.emplace(Language::czech, std::make_unique<LangFile>(Language::czech));
	}
}
