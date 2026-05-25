#include "hash.h"

std::string nosbazar::crypto::sha512(const std::string& input)
{
    unsigned char hash[SHA512_DIGEST_LENGTH];

    SHA512(
        reinterpret_cast<const unsigned char*>(input.data()),
        input.size(),
        hash
    );

    std::ostringstream oss;

    for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
        oss << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(hash[i]);
    }

    return oss.str();
}

std::string nosbazar::crypto::sha256(const std::string& input)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256(
        reinterpret_cast<const unsigned char*>(input.data()),
        input.size(),
        hash
    );

    std::ostringstream oss;

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        oss << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(hash[i]);
    }

    return oss.str();
}

std::string nosbazar::crypto::sha1(const std::string& input)
{
    unsigned char hash[SHA_DIGEST_LENGTH];

    SHA1(
        reinterpret_cast<const unsigned char*>(input.data()),
        input.size(),
        hash
    );

    std::ostringstream oss;

    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        oss << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(hash[i]);
    }

    return oss.str();
}
