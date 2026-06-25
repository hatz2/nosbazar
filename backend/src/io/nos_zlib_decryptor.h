#pragma once

#include <vector>
#include <cstdint>

namespace nosbazar::io {

class NosZLibDecryptor {
public:
    NosZLibDecryptor() = default;

    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data);
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data);
};

} // namespace nosbazar::io
