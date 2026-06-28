#pragma once

#include "lang_file_parser.h"
#include "nos_file_text_reader.h"
#include "language.h"
#include <nlohmann/json.hpp>

namespace nosbazar::io {
	class LangManager {
	public:
		static LangManager& get_instance();

		std::string get_item_translation(Language lang, const std::string& code_name);

		nlohmann::json get_all_item_translations(const std::string& code_name);

		std::string get_bcard_translation(Language lang, const std::string& code_name);

		nlohmann::json get_all_bcard_translations(const std::string& code_name);

	private:

		std::string get_translation(Language lang, const std::string& filename, const std::string& code_name);

		nlohmann::json get_all_translations(const std::string& filename_template, const std::string& code_name);
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
