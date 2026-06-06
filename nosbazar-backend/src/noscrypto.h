#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>

namespace nosbazar::noscrypto {

    class Client
    {
    public:

        [[nodiscard]] static std::vector<uint8_t> login_encrypt(const std::vector<uint8_t> packet);

        [[nodiscard]] static std::vector<uint8_t> login_decrypt(const std::vector<uint8_t>& packet);

        [[nodiscard]] static std::vector<uint8_t> world_encrypt(const std::vector<uint8_t>& packet);

        [[nodiscard]] static std::vector<uint8_t> world_decrypt(const std::vector<uint8_t>& packet, uint32_t session, bool is_first_packet = false);

        [[nodiscard]] static std::vector<uint8_t> world_xor(const std::vector<uint8_t>& packet, uint32_t session, bool is_first_packet);

        [[nosicard]] static std::vector<uint8_t> unpack(const std::vector<uint8_t>& packet);
    private:
        static inline constexpr std::array<uint8_t, 16> encryption_table =
        {
            0x00, 0x20, 0x2D, 0x2E,
            0x30, 0x31, 0x32, 0x33,
            0x34, 0x35, 0x36, 0x37,
            0x38, 0x39, 0x0A, 0x00
        };

        static inline constexpr std::array<uint8_t, 16> decryption_table =
        {
            0x00, 0x20, 0x2D, 0x2E,
            0x30, 0x31, 0x32, 0x33,
            0x34, 0x35, 0x36, 0x37,
            0x38, 0x39, 0xFF, 0x00
        };
    };

    class Server
    {
    public:

        [[nodiscard]] static std::vector<uint8_t> login_encrypt(std::vector<uint8_t> packet);

        [[nodiscard]] static std::vector<uint8_t> login_decrypt(const std::vector<uint8_t>& packet);

        [[nodiscard]] static std::vector<uint8_t> world_encrypt(const std::vector<uint8_t>& packet, uint32_t session, bool is_first_packet = false);

        [[nodiscard]] static std::vector<uint8_t> world_decrypt(const std::vector<uint8_t>& packet);

    private:

        static inline constexpr std::array<uint8_t, 16> encryption_table =
        {
            0x00, 0x20, 0x2D, 0x2E,
            0x30, 0x31, 0x32, 0x33,
            0x34, 0x35, 0x36, 0x37,
            0x38, 0x39, 0xFF, 0x00
        };

        static inline constexpr std::array<uint8_t, 16> decryption_table =
        {
            0x00, 0x20, 0x2D, 0x2E,
            0x30, 0x31, 0x32, 0x33,
            0x34, 0x35, 0x36, 0x37,
            0x38, 0x39, 0x0A, 0x00
        };

        static std::vector<uint8_t> world_xor(const std::vector<uint8_t>& packet, uint32_t session, bool is_first_packet);
    };
}