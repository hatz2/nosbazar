#include "net.h"
#include <spdlog/spdlog.h>

using namespace nosbazar::net;

namespace {
    size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
    {
        size_t total_size = size * nmemb;
        std::string* response = static_cast<std::string*>(userp);
        response->append(static_cast<char*>(contents), total_size);

        return total_size;
    }
}


std::expected<Response, Error> nosbazar::net::post(std::string_view url, std::string_view body, const std::vector<std::string>& headers)
{
    Response response;
    CURL* curl = curl_easy_init();

    if (!curl) {
        return std::unexpected(Error::curl_init);
    }

    std::string response_body;

    // URL
    curl_easy_setopt(curl, CURLOPT_URL, url.data());

    // Enable POST
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Body
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.data());

    // Headers
    struct curl_slist* curl_headers = nullptr;
    for (const auto& header : headers) {
        curl_headers = curl_slist_append(curl_headers, header.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);

    // Callback response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_body);

    // Execute request
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        SPDLOG_ERROR("curl_easy_perform() failed: {}", curl_easy_strerror(res));

        curl_slist_free_all(curl_headers);
        curl_easy_cleanup(curl);

        return std::unexpected(Error::curl_easy_perform);
    }
    else
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.status_code);
        response.body = std::move(response_body);
    }

    // Cleanup
    curl_slist_free_all(curl_headers);
    curl_easy_cleanup(curl);

    return response;
}

std::expected<Response, Error> nosbazar::net::get(std::string_view url, const std::vector<std::string>& headers)
{
    Response response;
    CURL* curl = curl_easy_init();

    if (!curl) {
        return std::unexpected(Error::curl_init);
    }

    std::string data;

    // URL
    curl_easy_setopt(curl, CURLOPT_URL, url.data());

    // Enable GET
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    // Headers
    struct curl_slist* curl_headers = nullptr;
    for (const auto& header : headers) {
        curl_headers = curl_slist_append(curl_headers, header.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);

    // Callback response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    // Execute request
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        SPDLOG_ERROR("curl_easy_perform() failed: {}", curl_easy_strerror(res));

        curl_slist_free_all(curl_headers);
        curl_easy_cleanup(curl);

        return std::unexpected(Error::curl_easy_perform);
    }
    else
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.status_code);
        response.body = std::move(data);
    }

    // Cleanup
    curl_slist_free_all(curl_headers);
    curl_easy_cleanup(curl);

    return response;
}
