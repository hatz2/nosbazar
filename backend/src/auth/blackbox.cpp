#include "blackbox.h"
#include <crypto/base64.h>
#include <crypto/hash.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <random/random.h>
#include <openssl/sha.h>
#include <unordered_set>

namespace {
    static bool is_unreserved(char c)
    {
        // "-_!~*.'()"
        const std::unordered_set<char> unreserved = { '-', '_', '!', '~', '*', '.', '\'', '(', ')' };

        if (unreserved.contains(c)) {
            return true;
        }

        return std::isalnum(static_cast<unsigned char>(c));
    }

    std::string percent_encode(const std::string& input)
    {
        std::ostringstream oss;

        for (unsigned char c : input) {
            if (is_unreserved(c)) {
                oss << c;
            }
            else {
                oss << '%' << std::uppercase << std::hex
                    << std::setw(2) << std::setfill('0')
                    << (int)c;
            }
        }

        return oss.str();
    }

    std::string to_url_safe_base64(std::string s)
    {
        for (char& c : s) {
            if (c == '/') {
                c = '_';
            }
            else if (c == '+') {
                c = '-';
            }
        }

        while (!s.empty() && s.back() == '=') {
            s.pop_back();
        }

        return s;
    }
}

using json = nlohmann::json;

nosbazar::auth::Blackbox::Blackbox(std::shared_ptr<Identity> identity) : identity(identity)
{

}



std::string nosbazar::auth::Blackbox::encoded() const
{
	json fingerprint_array = json::array();
	json fingerprint = identity->json();

	for (const auto& field : Blackbox::fields) {
		fingerprint_array.push_back(fingerprint[field]);
	}

	std::string fingerprint_array_str = fingerprint_array.dump();

    std::string uri_encoded = percent_encode(fingerprint_array_str);
    std::vector<uint8_t> blackbox;
    blackbox.reserve(uri_encoded.size());
    blackbox.push_back(static_cast<uint8_t>(uri_encoded[0]));

    for (size_t i = 1; i < uri_encoded.size(); ++i) {
        uint8_t a = blackbox[i - 1];
        uint8_t b = static_cast<uint8_t>(uri_encoded[i]);

        uint8_t c = static_cast<uint8_t>(a + b);

        blackbox.push_back(c);
    }

    std::string result = crypto::base64_encode(blackbox);
    result = to_url_safe_base64(result);
    return "tra:" + result;
}

nosbazar::auth::EncryptedBlackbox::EncryptedBlackbox(std::shared_ptr<Identity> identity, std::string account_id, std::string gsid, std::string installation_id)
    : Blackbox(identity)
    , gsid(gsid)
    , account_id(account_id)
{
    json request = {
        {"features", {random::random_int(1, INT32_MAX)}},
        {"installation",  installation_id},
        {"session", gsid.substr(0, gsid.find_last_of('-'))}
    };

    identity->set_request(request.dump());
}

std::string nosbazar::auth::EncryptedBlackbox::encrypted() const
{
    std::string key = gsid + "-" + account_id;
    key = crypto::sha512(key);

    std::string blackbox = encoded();

    std::vector<uint8_t> encrypted_blackbox;

    for (size_t i = 0; i < blackbox.size(); ++i) {
        size_t key_index = i % key.size();
        uint8_t val = blackbox[i] ^ key[key_index] ^ key[key.size() - key_index - 1];
        encrypted_blackbox.push_back(val);
    }

    return crypto::base64_encode(encrypted_blackbox);
}
