#include "base64.h"
#include <iostream>

std::vector<uint8_t> nosbazar::crypto::base64_decode(std::string input)
{
    std::vector<uint8_t> output((input.size() * 3) / 4);

    const int decoded_len = EVP_DecodeBlock(
        output.data(),
        reinterpret_cast<const unsigned char*>(input.data()),
        static_cast<int>(input.size())
    );

    if (decoded_len < 0)
        return {};

    size_t padding = 0;

    if (!input.empty() && input[input.size() - 1] == '=')
        padding++;

    if (input.size() > 1 && input[input.size() - 2] == '=')
        padding++;

    output.resize(decoded_len - padding);

    return output;
}

std::string nosbazar::crypto::base64_encode(const std::vector<uint8_t>& data)
{
    const auto encoded_len = 4 * ((data.size() + 2) / 3);

    std::string output(encoded_len, '\0');

    EVP_EncodeBlock(
        reinterpret_cast<unsigned char*>(output.data()),
        data.data(),
        static_cast<int>(data.size())
    );

    return output;
}
