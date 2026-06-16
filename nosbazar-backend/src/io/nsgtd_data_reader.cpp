#include "nsgtd_data_reader.h"
#include "file_reader.h"
#include "nos_text_dat_file_decryptor.h"
#include <spdlog/spdlog.h>

namespace {
#pragma pack(push, 1)
    struct FileData {
        uint32_t file_number{};
        uint32_t filename_size{};
        std::string filename;
        uint32_t is_dat{};
        uint32_t data_size{};
        std::vector<uint8_t> data;
    };
#pragma off
}

nosbazar::io::NSgtdDataReader::NSgtdDataReader(const std::string& file_path)
{
	read(file_path);
}

nosbazar::io::NSgtdDataReader::NSgtdDataReader() : NSgtdDataReader("assets/NostaleData/NSgtdData.NOS")
{
}

void nosbazar::io::NSgtdDataReader::read(const std::string& file_path)
{
    std::ifstream file(file_path, std::fstream::binary);

    if (!file.is_open()) {
        SPDLOG_DEBUG("Could not open file {} for read", file_path);
        return;
    }

    io::NosTextDatFileDecryptor decryptor;
    uint32_t number_of_files = 0;

    io::read_value(file, number_of_files);

    for (uint32_t i = 0; i < number_of_files && !file.eof(); ++i) {
        FileData file_info;

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
            
            if (file_info.filename == "Item.dat") {
                item_dat_file = std::move(std::string(file_info.data.begin(), file_info.data.end()));
            }
        }
    }
}
