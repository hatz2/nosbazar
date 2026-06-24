#include "encoding.h"
#include <unicode/unistr.h>
#include <unordered_map>

namespace {
    std::unordered_map<nosbazar::strings::Encoding, std::string> encoding_strings = {
        {nosbazar::strings::Encoding::windows1250, "windows-1250"},
        {nosbazar::strings::Encoding::windows1251, "windows-1251"},
        {nosbazar::strings::Encoding::windows1252, "windows-1252"},
        {nosbazar::strings::Encoding::windows1254, "windows-1254"},
    };
}

namespace nosbazar::strings {
    std::string convert_to_utf8(const std::string& input, Encoding input_encoding)
    {
        icu::UnicodeString unicode(input.c_str(), encoding_strings.at(input_encoding).c_str());

        std::string utf8;
        unicode.toUTF8String(utf8);

        return utf8;
    }
}