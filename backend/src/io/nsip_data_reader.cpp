#include "nsip_data_reader.h"
#include "nos_zlib_decryptor.h"
#include <fstream>
#include <stdexcept>
#include "file_reader.h"

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

        if (!decompressed_data.empty()) {
            icon_cache[entry.id] = std::move(decompressed_data);
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
