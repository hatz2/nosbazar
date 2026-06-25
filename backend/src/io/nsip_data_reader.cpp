#include "nsip_data_reader.h"
#include "nos_zlib_decryptor.h"
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include "file_reader.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace nosbazar::io {

NSipDataReader& NSipDataReader::instance() {
    static NSipDataReader instance;
    return instance;
}

void NSipDataReader::initialize(const std::filesystem::path& file_path) {
    if (initialized) {
        return;
    }

    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open NSipData file at: " + file_path.string());
    }

    // Skip the 16-byte header
    file.seekg(0x10, std::ios::beg);

    uint32_t num_entries = 0;
    
    if (!read_value(file, num_entries)) {
        throw std::runtime_error("Failed to read entry count from NSipData file.");
    }
    
    // 1 unknown byte
    uint8_t unknown;
    read_value(file, unknown);

    struct HeaderEntry {
        uint32_t id;
        uint32_t offset;
    };

    std::vector<HeaderEntry> entries(num_entries);
    if (!read_bytes(file, entries.data(), num_entries)) {
        throw std::runtime_error("Failed to read entries from NSipData file.");
    }

    NosZLibDecryptor decryptor;
    icon_cache.reserve(num_entries);

    for (const auto& entry : entries) {
        // Seek to the data position based on the entry's offset
        file.seekg(entry.offset, std::ios::beg);

        if (file.fail()) {
            continue;
        }

#pragma pack(push, 1)
        struct RawEntry {
            uint32_t hex_date;
            uint32_t data_size;
            uint32_t compressed_size;
            bool is_packed;
        } raw_entry;
#pragma pack(pop)

        if (!read_value(file, raw_entry)) {
            throw std::runtime_error("Failed to read raw entry from NSipData file");
        }

        std::vector<uint8_t> compressed_data(raw_entry.compressed_size);
        if (!read_bytes(file, compressed_data.data(), raw_entry.compressed_size)) {
            continue;
        }

        std::vector<uint8_t> decompressed_data = decryptor.decrypt(compressed_data);

        if (decompressed_data.size() >= 13) {
            uint16_t width = decompressed_data[1] | (decompressed_data[2] << 8);
            uint16_t height = decompressed_data[3] | (decompressed_data[4] << 8);

            if (width > 0 && height > 0) {
                size_t num_pixels = width * height;
                size_t expected_pixel_bytes = num_pixels * 2;
                size_t actual_pixel_bytes = decompressed_data.size() - 13;

                if (actual_pixel_bytes >= expected_pixel_bytes) {
                    std::vector<uint8_t> rgba(num_pixels * 4);

                    for (size_t i = 0; i < num_pixels; ++i) {
                        uint8_t gb = decompressed_data[13 + i * 2];
                        uint8_t ar = decompressed_data[13 + i * 2 + 1];

                        uint8_t r = ar & 0xF;
                        uint8_t g = gb >> 4;
                        uint8_t b = gb & 0xF;
                        uint8_t a = ar >> 4;

                        size_t idx = i * 4;
                        rgba[idx + 0] = r * 0x11;
                        rgba[idx + 1] = g * 0x11;
                        rgba[idx + 2] = b * 0x11;
                        rgba[idx + 3] = a * 0x11;
                    }

                    int png_len = 0;
                    unsigned char* png = stbi_write_png_to_mem(
                        rgba.data(), width * 4, width, height, 4, &png_len);

                    if (png) {
                        std::vector<uint8_t> png_buffer(png, png + png_len);
                        STBIW_FREE(png);
                        icon_cache[entry.id] = std::move(png_buffer);
                    }
                }
            }
        }
    }

    initialized = true;
}

const std::vector<uint8_t>* NSipDataReader::get_icon_data(uint32_t icon_id) const {
    if (icon_cache.contains(icon_id)) {
        return &icon_cache.at(icon_id);
    }
    return nullptr;
}

} // namespace nosbazar::io
