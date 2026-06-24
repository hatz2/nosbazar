#include "encoding.h"
#include <Windows.h>

namespace nosbazar::strings {
    std::string cp1250_to_utf8(const std::string& input)
    {
        if (input.empty())
            return {};

        int wide_size = MultiByteToWideChar(
            1250,
            0,
            input.data(),
            static_cast<int>(input.size()),
            nullptr,
            0);

        std::wstring wide(wide_size, L'\0');

        MultiByteToWideChar(
            1250,
            0,
            input.data(),
            static_cast<int>(input.size()),
            wide.data(),
            wide_size);

        int utf8_size = WideCharToMultiByte(
            CP_UTF8,
            0,
            wide.data(),
            static_cast<int>(wide.size()),
            nullptr,
            0,
            nullptr,
            nullptr);

        std::string utf8(utf8_size, '\0');

        WideCharToMultiByte(
            CP_UTF8,
            0,
            wide.data(),
            static_cast<int>(wide.size()),
            utf8.data(),
            utf8_size,
            nullptr,
            nullptr);

        return utf8;
    }
}
