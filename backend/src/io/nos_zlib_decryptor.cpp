#include "nos_zlib_decryptor.h"
#include <zlib.h>
#include <stdexcept>
#include <vector>
#include <cstdint>

namespace nosbazar::io {

std::vector<uint8_t> NosZLibDecryptor::encrypt(const std::vector<uint8_t>& data) {
    if (data.empty()) return {};

    uLongf destLen = compressBound(static_cast<uLong>(data.size()));
    std::vector<uint8_t> compressedData(destLen);

    int res = compress(compressedData.data(), &destLen, data.data(), static_cast<uLong>(data.size()));
    
    if (res != Z_OK) {
        throw std::runtime_error("zlib compression failed");
    }

    compressedData.resize(destLen);
    return compressedData;
}

std::vector<uint8_t> NosZLibDecryptor::decrypt(const std::vector<uint8_t>& data) {
    if (data.empty()) return {};

    z_stream strm{};
    strm.next_in = const_cast<Bytef*>(data.data());
    strm.avail_in = static_cast<uInt>(data.size());

    if (inflateInit(&strm) != Z_OK) {
        throw std::runtime_error("inflateInit failed");
    }

    // Guard to ensure inflateEnd is always called
    struct InflateGuard {
        z_stream* s;
        ~InflateGuard() { inflateEnd(s); }
    } guard{&strm};

    std::vector<uint8_t> output;
    const size_t CHUNK = 4096;
    uint8_t buffer[CHUNK];

    int ret;
    do {
        strm.next_out = buffer;
        strm.avail_out = CHUNK;

        ret = inflate(&strm, Z_NO_FLUSH);

        if (ret != Z_OK && ret != Z_STREAM_END) {
            throw std::runtime_error("inflate failed");
        }

        size_t produced = CHUNK - strm.avail_out;
        if (produced > 0) {
            output.insert(output.end(), buffer, buffer + produced);
        }

    } while (ret != Z_STREAM_END);

    return output;
}

} // namespace nosbazar::io
