#include "nos_text_dat_file_decryptor.h"

nosbazar::io::NosTextDatFileDecryptor::NosTextDatFileDecryptor()
    : cryptoArray(
        { 0x00, 0x20, 0x2D, 0x2E, 0x30, 0x31, 0x32, 0x33,
         0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x0A, 0x00 }) {
}

std::vector<uint8_t> nosbazar::io::NosTextDatFileDecryptor::encrypt(const std::vector<uint8_t>& array) {
    std::vector<uint8_t> mask = getMask(array);
    std::vector<uint8_t> result;
    int iterator = 0;
    int size = static_cast<int>(mask.size());
    uint8_t switchByte = 0;

    while (iterator < size) {
        int len = iterator;

        while ((iterator < size && mask[iterator] == 0x30) ||
            (iterator + 1 == size) ||
            (iterator + 2 == size) ||
            (iterator + 1 < size && mask[iterator + 1] == 0x30) ||
            (iterator + 2 < size && mask[iterator + 2] == 0x30))
            ++iterator;

        if (iterator > len) {
            for (int j = iterator - len; j > 0; j -= 0x7E) {
                int checker = j;
                if (checker > 0x7E)
                    checker = 0x7E;

                result.push_back(static_cast<uint8_t>(checker));

                for (; checker > 0; --checker) {
                    uint8_t byteToAdd = array[len] ^ 0x33;
                    len++;
                    result.push_back(byteToAdd);
                }
            }
        }

        if (iterator >= size)
            break;

        len = iterator;
        int v25 = 1;

        while (iterator < size && mask[iterator] == 0x31)
            ++iterator;

        if (iterator > len) {
            for (int j = iterator - len; j > 0; j -= 0x7E) {
                int checker = j;
                if (j > 0x7E)
                    checker = 0x7E;

                result.push_back(static_cast<uint8_t>(checker | 0x80));

                for (; checker > 0; --checker) {
                    uint8_t byteToAdd = array[len];
                    len++;

                    switch (byteToAdd) {
                    case 32:  switchByte = 1;               break;
                    case 45:  switchByte = 2;               break;
                    case 46:  switchByte = 3;               break;
                    case 0xFF: switchByte = 14;             break;
                    default:  switchByte = byteToAdd - 0x2C; break;
                    }

                    if (v25) {
                        result.push_back(static_cast<uint8_t>(0x10 * switchByte));
                        v25 = 0;
                    }
                    else {
                        result.back() = result.back() | switchByte;
                        v25 = 1;
                    }
                }
            }
        }
    }

    result.push_back(0xFF);
    return result;
}

std::vector<uint8_t> nosbazar::io::NosTextDatFileDecryptor::decrypt(const std::vector<uint8_t>& array) {
    std::vector<uint8_t> decryptedFile;
    int currIndex = 0;
    int arraySize = static_cast<int>(array.size());

    while (currIndex < arraySize) {
        uint8_t currentByte = array[currIndex];
        currIndex++;

        if (currentByte == 0xFF) {
            decryptedFile.push_back(0x0D);
            continue;
        }

        int validate = currentByte & 0x7F;

        if (currentByte & 0x80) {
            for (; validate > 0; validate -= 2) {
                if (currIndex >= arraySize)
                    break;

                currentByte = array[currIndex];
                currIndex++;

                uint8_t firstByte = cryptoArray[(currentByte & 0xF0) >> 4];
                decryptedFile.push_back(firstByte);

                if (validate <= 1)
                    break;

                uint8_t secondByte = cryptoArray[currentByte & 0x0F];
                if (!secondByte)
                    break;

                decryptedFile.push_back(secondByte);
            }
        }
        else {
            for (; validate > 0; --validate) {
                if (currIndex >= arraySize)
                    break;

                currentByte = array[currIndex];
                currIndex++;
                decryptedFile.push_back(currentByte ^ 0x33);
            }
        }
    }

    return decryptedFile;
}

std::string nosbazar::io::NosTextDatFileDecryptor::decrypt(const std::string& array)
{
    std::vector<uint8_t> raw(array.begin(), array.end());
    raw = decrypt(std::move(raw));
    std::string result(raw.begin(), raw.end());
    return result;
}

std::vector<uint8_t> nosbazar::io::NosTextDatFileDecryptor::getMask(const std::vector<uint8_t>& array) {
    std::vector<uint8_t> mask(array.size(), 0x30);

    for (std::size_t i = 0; i < mask.size(); i++) {
        uint8_t ch = array[i];

        if (!ch)
            break;

        ch -= 0x20;
        if (ch) {
            ch -= 0x0D;
            if (ch >= 2) {
                ch -= 0x03;
                bool test1 = ch < 0x0A;
                ch -= 0x0A;
                if (!test1 && ch != 0xC5)
                    continue;
            }
        }

        mask[i] = 0x31;
    }

    return mask;
}