#include "uuid.h"

std::string nosbazar::crypto::generate_uuid_v4()
{
    std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";

    for (char& ch : uuid)
    {
        if (ch == 'x')
        {
            ch = random::random_uuid_character();
        }
        else if (ch == 'y')
        {
            // Variant RFC 4122: 8, 9, a or b
            static const char variants[] = { '8', '9', 'a', 'b' };
            ch = variants[random::random_int(0, 3)];
        }
    }

    return uuid;
}
