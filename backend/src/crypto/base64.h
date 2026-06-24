#pragma once

#include <openssl/evp.h>

#include <string>
#include <vector>

namespace nosbazar::crypto {
	std::vector<uint8_t> base64_decode(std::string input);

	std::string base64_encode(const std::vector<uint8_t>& data);
}