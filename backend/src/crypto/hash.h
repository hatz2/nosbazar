#pragma once

#include <openssl/sha.h>
#include <openssl/md5.h>
#include <string>
#include <sstream>
#include <iomanip>

namespace nosbazar::crypto {
    std::string sha512(const std::string& input);

    std::string sha256(const std::string& input);

    std::string sha1(const std::string& input);

    std::string md5(const std::string& input, bool uppercase = false);
}

