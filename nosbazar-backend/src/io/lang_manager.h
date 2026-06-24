#pragma once

#include "lang_file_parser.h"
#include "nos_file_text_reader.h"
#include <nlohmann/json.hpp>

namespace nosbazar::io {
	enum Language {
		spanish,
		english,
		french,
		german,
		turkish,
		italian,
		russian,
		polish,
		czech
	};

	static inline const std::unordered_map<Language, std::string> language_string = {
		{ Language::spanish, "ES" },
		{ Language::english, "UK" },
		{ Language::french, "FR" },
		{ Language::german, "DE" },
		{ Language::turkish, "TR" },
		{ Language::italian, "IT" },
		{ Language::russian, "RU" },
		{ Language::polish, "PL" },
		{ Language::czech, "CZ" }
	};

	static inline const std::unordered_map<std::string, Language> string_to_lang = {
		{ "ES", Language::spanish },
		{ "UK", Language::english },
		{ "FR", Language::french },
		{ "DE", Language::german },
		{ "TR", Language::turkish },
		{ "IT", Language::italian },
		{ "RU", Language::russian },
		{ "PL", Language::polish },
		{ "CZ", Language::czech }
	};

	class LangManager {
	public:
		static LangManager& get_instance();

		std::string get_item_translation(Language lang, const std::string& code_name);

		nlohmann::json get_all_item_translations(const std::string& code_name);

	private:
		/**
		 * @brief Helper class that represents a single NSlangData_XX.NOS file.
		 */
		class LangFile {
		public:
			LangFile(Language lang);

			std::string translation(const std::string& filename, const std::string& code_name);

		private:
			std::unordered_map<std::string /* filename */, std::unique_ptr<LangFileParser>> file_parsers;
		};

		LangManager();
		LangManager(const LangManager& other) = delete;
		void operator=(const LangManager& other) = delete;

		
		std::unordered_map<Language, std::unique_ptr<LangFile>> lang_files;
	};
}
