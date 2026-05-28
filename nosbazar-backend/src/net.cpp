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

nosbazar::net::TCPClient::TCPClient(asio::io_context& context) 
    : context(context)
    , socket(context)
    , read_buffer{}
{
}

void nosbazar::net::TCPClient::connect(const std::string& ip, unsigned short port)
{
    asio::ip::address addr = asio::ip::make_address(ip);
    auto endpoint = asio::ip::tcp::endpoint(addr, port);

    asio::error_code ec;

    socket.connect(endpoint, ec);

    if (ec) {
        SPDLOG_ERROR("TCPClient::connect error: {}", ec.message());
        observer.on_disconnect();
        return;
    }

    do_recv();
}

void nosbazar::net::TCPClient::send(const std::vector<uint8_t>& data)
{
    asio::error_code ec;
    size_t res = asio::write(socket, asio::buffer(data), ec);

    if (ec) {
        SPDLOG_ERROR("TCPClient::send error: {}", ec.message());
    }

    if (res != data.size()) {
        SPDLOG_ERROR("TCPClient::send tried to write {} bytes but only {} bytes were written", data.size(), res);
    }
}

void nosbazar::net::TCPClient::send(const std::string& data)
{
    std::vector<uint8_t> raw(data.begin(), data.end());
    send(std::move(raw));
}

void nosbazar::net::TCPClient::disconnect()
{
    socket.close();
}

void nosbazar::net::TCPClient::do_recv()
{
    socket.async_read_some(asio::buffer(read_buffer), [this](std::error_code ec, std::size_t length) -> void {
        if (ec) {
            if (ec != asio::error::eof) {
                SPDLOG_ERROR("TCPClient::do_recv: {}", ec.message());
            }
            
            observer.on_disconnect();
        }
        else {
            std::vector<uint8_t> data(read_buffer.begin(), read_buffer.end());
            observer.on_receive(std::move(data));
            do_recv();
        }
    });
}

nosbazar::net::Session::Session(std::unique_ptr<TCPClient> client) : client(std::move(client))
{
    std::function<void()> on_connect_cb = [this]() {
        on_connect();
    };

    std::function<void()> on_disconnect_cb = [this]() {
        on_disconnect();
    };

    std::function<void(std::vector<uint8_t>)> on_recv_cb = [this](std::vector<uint8_t> data) {
        on_recv(std::move(data));
    };

    this->client->observer = {
        on_connect_cb,
        on_recv_cb,
        on_disconnect_cb
    };

    this->client->send("0\n");
}

void nosbazar::net::Session::on_connect()
{
    SPDLOG_DEBUG("Session::on_connect");
}

void nosbazar::net::Session::on_disconnect()
{
    SPDLOG_DEBUG("Session::on_disconnect");
}

void nosbazar::net::Session::on_recv(std::vector<uint8_t> data)
{
    SPDLOG_DEBUG("Session::on_recv");
}
