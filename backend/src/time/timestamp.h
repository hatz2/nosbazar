#pragma once

#include <chrono>
#include <cstdint>

namespace nosbazar::time {
    inline uint64_t current_timestamp_ms()
    {
        using namespace std::chrono;

        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        ).count();
    }
}

