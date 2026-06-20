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

		for (const std::string& filename : filenames) {
			std::string content = reader.get_file_content(filename);
			auto parser = std::make_unique<LangFileParser>(content);
			file_parsers.emplace(filename, std::move(parser));
		}
	}
	std::string LangManager::LangFile::translation(const std::string& filename, const std::string& code_name)
	{
		if (file_parsers.contains(filename)) {
			auto& parser = file_parsers.at(filename);
			return parser->translation(code_name);
		}
		
		return code_name;
	}

	LangManager& LangManager::get_instance()
	{
		static LangManager instance;
		return instance;
	}

	std::string LangManager::get_item_translation(Language lang, const std::string& code_name)
	{
		std::string language = language_string.at(lang);
		std::transform(language.begin(), language.end(), language.begin(), ::tolower);
		std::string filename = fmt::format("_code_{}_Item.txt", language);
		return lang_files.at(lang)->translation(filename, code_name);
	}

	nlohmann::json LangManager::get_all_item_translations(const std::string& code_name)
	{
		nlohmann::json translation = {
			{language_string.at(Language::spanish), get_item_translation(Language::spanish, code_name)},
			{language_string.at(Language::english), get_item_translation(Language::english, code_name)},
			{language_string.at(Language::french), get_item_translation(Language::french, code_name)},
			{language_string.at(Language::german), get_item_translation(Language::german, code_name)},
			{language_string.at(Language::turkish), get_item_translation(Language::turkish, code_name)},
			{language_string.at(Language::italian), get_item_translation(Language::italian, code_name)},
			{language_string.at(Language::russian), get_item_translation(Language::russian, code_name)},
			{language_string.at(Language::polish), get_item_translation(Language::polish, code_name)},
			{language_string.at(Language::czech), get_item_translation(Language::czech, code_name)},
		};

		return translation;
	}

	LangManager::LangManager()
	{
		lang_files.emplace(Language::spanish, std::make_unique<LangFile>(Language::spanish));
		lang_files.emplace(Language::english, std::make_unique<LangFile>(Language::english));
		lang_files.emplace(Language::french, std::make_unique<LangFile>(Language::french));
		lang_files.emplace(Language::german, std::make_unique<LangFile>(Language::german));
		lang_files.emplace(Language::turkish, std::make_unique<LangFile>(Language::turkish));
		lang_files.emplace(Language::italian, std::make_unique<LangFile>(Language::italian));
		lang_files.emplace(Language::russian, std::make_unique<LangFile>(Language::russian));
		lang_files.emplace(Language::polish, std::make_unique<LangFile>(Language::polish));
		lang_files.emplace(Language::czech, std::make_unique<LangFile>(Language::czech));
	}
}
