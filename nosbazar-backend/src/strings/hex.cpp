#include "hex.h"

std::string nosbazar::strings::hexlify(const std::string& input)
{
    std::ostringstream oss;

    oss << std::hex << std::uppercase << std::setfill('0');

    for (unsigned char c : input)
    {
        oss << std::setw(2) << static_cast<int>(c);
    }

    return oss.str();
}
