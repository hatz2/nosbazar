#pragma once

#include <filesystem>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <optional>

namespace nosbazar::io {

class NSipDataReader {
public:
    // Singleton access
    static NSipDataReader& instance();

    /**
     * @brief Initializes the reader by parsing the entire NSipData.NOS file
     * and decompressing all icon images into memory.
     * 
     * @param file_path The absolute path to the NSipData.NOS file.
     * @throws std::runtime_error if the file cannot be opened or parsed.
     */
    void initialize(const std::filesystem::path& file_path);

    /**
     * @brief Returns a pointer to the decompressed PNG data for a given icon ID.
     * 
     * @param icon_id The ID of the icon to retrieve.
     * @return const std::vector<uint8_t> Pointer to the data buffer, or nullptr if not found.
     */
    const std::vector<uint8_t>* get_icon_data(uint32_t icon_id) const;

private:
    NSipDataReader() = default;
    // Delete copy and assignment to maintain Singleton integrity
    NSipDataReader(const NSipDataReader&) = delete;
    NSipDataReader& operator=(const NSipDataReader&) = delete;

    std::unordered_map<uint32_t, std::vector<uint8_t>> icon_cache;
    bool initialized = false;
};

} // namespace nosbazar::io
