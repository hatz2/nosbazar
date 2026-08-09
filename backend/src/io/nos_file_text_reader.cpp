#include "nos_file_text_reader.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include "nos_text_dat_file_decryptor.h"
#include "file_reader.h"

namespace nosbazar::io {
	NosFileTextReader::NosFileTextReader(const std::string& file_path)
	{
		read(file_path);
	}

	const TextFileData& NosFileTextReader::get_file(const std::string& filename)
	{
        if (files.contains(filename)) {
            return files.at(filename);
        }
        else {
            SPDLOG_ERROR("Could not find file with name: {}", filename);
            return files.begin()->second;
        }
        
	}
    std::string NosFileTextReader::get_file_content(const std::string& filename)
    {
        if (files.contains(filename)) {
            return files.at(filename).data;
        }
        else {
            SPDLOG_DEBUG("Could not find file with name: {}", filename);
            return {};
        }
    }
    std::vector<std::string> NosFileTextReader::get_filenames() const
    {
        std::vector<std::string> filenames;

        for (const auto& [key, _] : files) {
            filenames.push_back(key);
        }

        return filenames;
    }

	void NosFileTextReader::read(const std::string& file_path)
	{
        SPDLOG_INFO("Reading file {}...", file_path);
        std::ifstream file(file_path, std::fstream::binary);

        if (!file.is_open()) {
            SPDLOG_DEBUG("Could not open file {} for read", file_path);
            return;
        }

        io::NosTextDatFileDecryptor decryptor;
        uint32_t number_of_files = 0;

        io::read_value(file, number_of_files);

        for (uint32_t i = 0; i < number_of_files && !file.eof(); ++i) {
            TextFileData file_info;

            io::read_value(file, file_info.file_number);
            io::read_value(file, file_info.filename_size);
            file_info.filename.resize(file_info.filename_size);
            io::read_bytes(file, file_info.filename.data(), file_info.filename.size());
            io::read_value(file, file_info.is_dat);
            io::read_value(file, file_info.data_size);
            file_info.data.resize(file_info.data_size);
            io::read_bytes(file, file_info.data.data(), file_info.data.size());

            if (file_info.is_dat) {
                file_info.data = decryptor.decrypt(std::move(file_info.data));
            }

            files.emplace(file_info.filename, std::move(file_info));
        }
	}
}


