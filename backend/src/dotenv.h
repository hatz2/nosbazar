#pragma once

#include <cstdlib>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <spdlog/spdlog.h>

namespace dotenv {
	inline std::string trim(std::string value) {
		auto start = value.find_first_not_of(" \t\r");
		if (start == std::string::npos) {
			return {};
		}
		auto end = value.find_last_not_of(" \t\r");
		return value.substr(start, end - start + 1);
	}

	inline void init() {
		static bool loaded = false;
		if (loaded) {
			return;
		}
		loaded = true;

        std::ifstream file(".env");

        if (!file.is_open()) {
            return;
        }

        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            auto pos = line.find('=');

            if (pos == std::string::npos) {
                continue;
            }

            std::string key = dotenv::trim(line.substr(0, pos));
            std::string value = dotenv::trim(line.substr(pos + 1));

            if (!value.empty() && ((value.front() == '"' && value.back() == '"') ||
                                   (value.front() == '\'' && value.back() == '\''))) {
                value = value.substr(1, value.size() - 2);
            }

#ifdef _WIN32
            _putenv_s(key.c_str(), value.c_str());
#else
            setenv(key.c_str(), value.c_str(), 1);
#endif
        }
	}

    inline std::optional<std::string> get(std::string_view key) {
        const char* buffer = std::getenv(key.data());

        if (buffer == nullptr) {
            return std::nullopt;
        }

        return std::string(buffer);
    }
}