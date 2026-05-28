#pragma once

#include <curl/curl.h>
#include <string>
#include <string_view>
#include <expected>
#include <vector>
#include <asio.hpp>

namespace nosbazar::net {
	struct Response {
		int status_code;
		std::string body;
	};

	enum class Error {
		curl_init,
		curl_easy_perform,
	};

	std::expected<Response, Error> post(std::string_view url, std::string_view body, const std::vector<std::string>& headers = {});

	std::expected<Response, Error> get(std::string_view url, const std::vector<std::string>& headers = {});

	class TCPClient {
	public:
		struct Observer {
			std::function<void()> on_connect;
			std::function<void(std::vector<uint8_t>)> on_receive;
			std::function<void()> on_disconnect;
		} observer;

		explicit TCPClient(asio::io_context& context);

		void connect(const std::string& ip, unsigned short port);

		void send(const std::vector<uint8_t>& data);

		void send(const std::string& data);

		void disconnect();

	private:
		void do_recv();

		std::reference_wrapper<asio::io_context> context;
		asio::ip::tcp::socket socket;
		std::array<uint8_t, 4096> read_buffer;
	};

	class Session {
	public:
		explicit Session(std::unique_ptr<TCPClient> client);

		void on_connect();

		void on_disconnect();

		void on_recv(std::vector<uint8_t> data);

	private:
		TCPClient::Observer observer;
		std::unique_ptr<TCPClient> client;
	};


}