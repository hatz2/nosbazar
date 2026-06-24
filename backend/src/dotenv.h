#pragma once

#include <fstream>
#include <string>
#include <optional>

namespace dotenv {
	inline void init() {
        std::ifstream file(".env");

        std::string line;

        while (std::getline(file, line)) {
            auto pos = line.find('=');

            if (pos == std::string::npos)
                continue;

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

#ifdef _WIN32
            _putenv_s(key.c_str(), value.c_str());
#else
            setenv(key.c_str(), value.c_str(), 1);
#endif
        }
	}

    inline std::optional<std::string> get(std::string_view key) {
        char* buffer = nullptr;
        size_t size = 0;

        if (_dupenv_s(&buffer, &size, key.data()) != 0 || buffer == nullptr) {
            return std::nullopt;
        }

        std::string value(buffer);

        free(buffer);

        return value;
    }
}