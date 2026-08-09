#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace nosbazar::io {
#pragma pack(push, 1)
	struct TextFileData {
		uint32_t file_number{};
		uint32_t filename_size{};
		std::string filename;
		uint32_t is_dat{};
		uint32_t data_size{};
		std::string data;
	};
#pragma pack(pop)

	class NosFileTextReader {
	public:
		NosFileTextReader(const std::string& file_path);

		const TextFileData& get_file(const std::string& filename);

		std::string get_file_content(const std::string& filename);

		std::vector<std::string> get_filenames() const;

	private:
		void read(const std::string& file_path);

		std::unordered_map<std::string /* filename */, TextFileData> files;
	};
}
