#pragma once
 
#include <string>
 
namespace nosbazar::strings {
    enum class Encoding {
        windows1250,
        windows1251,
        windows1252,
        windows1254
    };

    std::string convert_to_utf8(const std::string& input, Encoding input_encoding);
}

