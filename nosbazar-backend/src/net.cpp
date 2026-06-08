#include "net.h"
#include <spdlog/spdlog.h>
#include <noscrypto.h>
#include <strings/parse.h>
#include <random/random.h>

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


std::expected<Response, CurlError> nosbazar::net::post(std::string_view url, std::string_view body, const std::vector<std::string>& headers)
{
    Response response;
    CURL* curl = curl_easy_init();

    if (!curl) {
        return std::unexpected(CurlError::curl_init);
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

        return std::unexpected(CurlError::curl_easy_perform);
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

std::expected<Response, CurlError> nosbazar::net::get(std::string_view url, const std::vector<std::string>& headers)
{
    Response response;
    CURL* curl = curl_easy_init();

    if (!curl) {
        return std::unexpected(CurlError::curl_init);
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

        return std::unexpected(CurlError::curl_easy_perform);
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
            std::vector<uint8_t> data(read_buffer.begin(), read_buffer.begin() + length);
            observer.on_receive(std::move(data));
            do_recv();
        }
    });
}

nosbazar::net::Session::Session(std::unique_ptr<TCPClient> client, packets::Publisher& publisher) : client(std::move(client)), publisher(publisher)
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
}

void nosbazar::net::Session::on_connect()
{
    SPDLOG_DEBUG("Session::on_connect");
}

void nosbazar::net::Session::on_disconnect()
{
    SPDLOG_DEBUG("Session::on_disconnect");
}

void nosbazar::net::Session::on_packet(const std::string& packet)
{
    publisher.publish(packet);
}

nosbazar::net::LoginSession::LoginSession(std::unique_ptr<TCPClient> client, packets::Publisher& publisher)
    : Session(std::move(client), publisher)
    , acumulator(0xA)
{
}

void nosbazar::net::LoginSession::send(const std::string& packet)
{
    std::vector<uint8_t> raw(packet.begin(), packet.end());
    std::vector<uint8_t> encrypted = noscrypto::Client::login_encrypt(raw);
    client->send(encrypted);
}

void nosbazar::net::LoginSession::on_recv(std::vector<uint8_t> data)
{
    std::vector<uint8_t> decrypted = noscrypto::Client::login_decrypt(data);
    acumulator.process(decrypted, pending_packets);

    while (!pending_packets.empty()) {
        auto raw_packet = std::move(pending_packets.front());
        std::string packet(raw_packet.begin(), raw_packet.end());
        pending_packets.pop();

        on_packet(packet);
    }
}

nosbazar::net::WorldSession::WorldSession(std::unique_ptr<TCPClient> client, packets::Publisher& publisher, uint16_t session_id)
    : Session(std::move(client), publisher)
    , session_id(session_id)
    , acumulator(0xFF)
    , packet_counter(random::random_int(static_cast<uint16_t>(0), UINT16_MAX))
{
}

void nosbazar::net::WorldSession::send(const std::string& packet)
{
    static bool is_first_packet = true;

    std::string packet_with_count = fmt::format("{} {}", packet_counter++, packet);
    std::vector<uint8_t> raw(packet_with_count.begin(), packet_with_count.end());
    std::vector<uint8_t> encrypted = noscrypto::Client::world_encrypt(raw, session_id, is_first_packet);
    client->send(encrypted);

    is_first_packet = false;
}

void nosbazar::net::WorldSession::on_recv(std::vector<uint8_t> data)
{
    acumulator.process(data, pending_packets);

    // Process pending packets
    while (!pending_packets.empty()) {
        auto raw_packet = std::move(pending_packets.front());
        auto unpacked = noscrypto::Client::world_decrypt(raw_packet);
        std::string packet(unpacked.begin(), unpacked.end());
        pending_packets.pop();

        if (packet.back() == '\n') {
            packet.pop_back();
        }

        on_packet(packet);
    }
}

nosbazar::net::PacketAcumulator::PacketAcumulator(uint8_t delimiter) : delimiter(delimiter)
{
}

void nosbazar::net::PacketAcumulator::process(std::span<const uint8_t> data, std::queue<std::vector<uint8_t>>& out_queue)
{
    auto it = data.begin();
    auto end = data.end();

    // 1. Complete any pending packet if it exists
    if (!buffer.empty()) {
        auto delimiter_it = std::ranges::find(it, end, delimiter);

        if (delimiter_it != end) {
            // Complete the previously buffered packet
            buffer.insert(buffer.end(), it, delimiter_it);

            out_queue.emplace(buffer.begin(), buffer.end());
            buffer.clear();

            it = std::next(delimiter_it);  // advance past the delimiter
        }
        else {
            // Still incomplete, append everything to the buffer
            buffer.insert(buffer.end(), it, end);
            return;
        }
    }

    // 2. Process complete packets directly from the span
    while (it != end) {
        auto delimiter_it = std::ranges::find(it, end, delimiter);
        if (delimiter_it == end) {
            break;
        }

        out_queue.emplace(it, delimiter_it);
        it = std::next(delimiter_it);
    }

    // 3. Store remaining incomplete tail
    if (it != end) {
        buffer.insert(buffer.end(), it, end);
    }
}

void nosbazar::net::PacketAcumulator::set_delimiter(uint8_t delimiter)
{
    this->delimiter = delimiter;
}
