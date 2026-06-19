#include "lang_manager.h"
#include <spdlog/spdlog.h>
#include <algorithm>

namespace nosbazar::io {
	LangManager::LangFile::LangFile(Language lang)
	{
		std::string language = language_string.at(lang);
		std::string uppercase_lang;
		std::transform(language.begin(), language.end(), uppercase_lang.begin(), std::toupper);
		std::string filepath = fmt::format("assets/NostaleData/NSlangData_{}.NOS", uppercase_lang);

		this->reader = std::make_unique<NosFileTextReader>(filepath);

		std::vector<std::string> filenames = reader->get_filenames();

		for (const std::string& filename : filenames) {
			std::string content = reader->get_file_content(filename);
			LangFileParser parser(content);
			file_parsers.emplace(filename, std::move(parser));
		}
	}
	std::string LangManager::LangFile::translation(const std::string& filename, const std::string& code_name)
	{
		if (file_parsers.contains(filename)) {
			auto& parser = file_parsers.at(filename);
			return parser.translation(code_name);
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
		std::transform(language.begin(), language.end(), language.begin(), std::tolower);
		std::string filename = fmt::format("_code_{}_Item.txt", language);
		return lang_files.at(lang).translation(filename, code_name);
	}

	LangManager::LangManager()
	{
		this->lang_files = {
			{Language::spanish, std::move(LangFile(Language::spanish))},
			{Language::english, std::move(LangFile(Language::english))},
			{Language::french, std::move(LangFile(Language::french))},
			{Language::german, std::move(LangFile(Language::german))},
			{Language::turkish, std::move(LangFile(Language::turkish))},
			{Language::italian, std::move(LangFile(Language::italian))},
			{Language::russian, std::move(LangFile(Language::russian))},
			{Language::polish, std::move(LangFile(Language::polish))},
			{Language::czech, std::move(LangFile(Language::czech))},
		};
	}
}
