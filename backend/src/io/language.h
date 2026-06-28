#pragma once

#include <string>
#include <unordered_map>
#include <strings/encoding.h>

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

inline strings::Encoding encoding_for(Language lang)
{
	switch (lang) {
	case Language::russian:
		return strings::Encoding::windows1251;
	case Language::spanish:
	case Language::french:
	case Language::english:
		return strings::Encoding::windows1252;
	case Language::turkish:
		return strings::Encoding::windows1254;
	default:
		return strings::Encoding::windows1250;
	}
}

}
