#pragma once

#include <random>

namespace nosbazar::random {

    inline std::mt19937& rng()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }

    template <typename T>
    T random_int(T min, T max)
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(rng());
    }

    template <typename T>
    T random_real(T min, T max)
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(rng());
    }

    inline bool random_bool() {
        return random_int(0, 1);
    }

    inline char random_character(char min, char max) {
        int character = random_int(static_cast<int>(min), static_cast<int>(max));
        return static_cast<char>(character);
    }

    inline char random_ascii_character() {
        int character = random_int(33, 126);
        return static_cast<char>(character);
    }

    inline char random_uuid_character() {
        bool number = random_bool();
        return number ? random_character('0', '9') : random_character('a', 'f');
    }

    inline std::string random_hex_string(size_t size) {
        static constexpr char hex_chars[] = "0123456789ABCDEF";
        constexpr int last_hex_char_idx = 15;
        std::string result;

        for (size_t i = 0; i < size; ++i) {
            result += hex_chars[random_int(0, last_hex_char_idx)];
        }

        return result;
    }

    inline std::string random_character_name(size_t size) {
        std::string result;

        for (size_t i = 0; i < size; ++i) {
            result += random_character('a', 'z');
        }

        return result;
    }
}
