#pragma once

#include <cstdlib>
#include <fstream>
#include <optional>
#include <string>
#include <string_view>
#include <spdlog/spdlog.h>

namespace dotenv {
	inline void init() {
		static bool loaded = false;
		if (loaded) {
			return;
		}
		loaded = true;

        std::ifstream file(".env");

        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            auto pos = line.find('=');

            if (pos == std::string::npos) {
                continue;
            }

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

#ifdef _WIN32
            SPDLOG_DEBUG("Setting env variable {}={}", key, value);
            _putenv_s(key.c_str(), value.c_str());
#else
            SPDLOG_DEBUG("Setting env variable {}={}", key, value);
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