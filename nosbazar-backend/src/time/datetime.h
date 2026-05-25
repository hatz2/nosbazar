#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace nosbazar::time {
    inline std::string current_datetime_iso8601_utc_ms()
    {
        using namespace std::chrono;

        const auto now = system_clock::now();

        const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

        const std::time_t time = system_clock::to_time_t(now);

        std::tm utc_time{};

#ifdef _WIN32
        gmtime_s(&utc_time, &time);
#else
        gmtime_r(&time, &utc_time);
#endif

        std::ostringstream oss;

        oss << std::put_time(&utc_time, "%Y-%m-%dT%H:%M:%S")
            << '.'
            << std::setfill('0')
            << std::setw(3)
            << ms.count()
            << 'Z';

        return oss.str();
    }
}

