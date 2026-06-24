/*****************************************************************//**
 * \file   nos_text_dat_file_decryptor.h
 * \brief  AI translated code from https://github.com/Pumba98/OnexExplorer/blob/master/Source/Decryptors/NosTextDatFileDecryptor.h
 * 
 * \date   June 2026
 *********************************************************************/
#pragma once

#include <array>
#include <vector>
#include <cstdint>
#include <string>

namespace nosbazar::io {
    class NosTextDatFileDecryptor {
    public:
        NosTextDatFileDecryptor();

        std::vector<uint8_t> encrypt(const std::vector<uint8_t>& array);
        std::vector<uint8_t> decrypt(const std::vector<uint8_t>& array);
        std::string decrypt(const std::string& array);

    private:
        std::array<uint8_t, 16> cryptoArray;
        std::vector<uint8_t> getMask(const std::vector<uint8_t>& array);
    };
}

