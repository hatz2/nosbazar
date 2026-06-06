#pragma once

#include <curl/curl.h>
#include <string>
#include <string_view>
#include <expected>
#include <vector>
#include <asio.hpp>
#include <span>
#include <queue>
#include <unordered_map>

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

	class PacketAcumulator {
	public:
		explicit PacketAcumulator(uint8_t delimiter);

		void process(std::span<const uint8_t> data, std::queue<std::vector<uint8_t>>& out_queue);

		void set_delimiter(uint8_t delimiter);

	private:
		uint8_t delimiter;
		std::vector<uint8_t> buffer;
	};

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
		using PacketHandler = std::function<void(const std::string&)>;

		explicit Session(std::unique_ptr<TCPClient> client);

		virtual void send(const std::string& packet) = 0;

		void subscribe(std::string_view packet_header, PacketHandler handler);

	protected:
		void on_connect();
		void on_disconnect();
		virtual void on_recv(std::vector<uint8_t> data) = 0;
		void on_packet(const std::string& packet);

		std::unique_ptr<TCPClient> client;
		std::queue<std::vector<uint8_t>> pending_packets;

	private:
		TCPClient::Observer observer;
		std::unordered_map< std::string_view, std::vector<PacketHandler>> handlers;
	};

	class LoginSession : public Session {
	public:
		explicit LoginSession(std::unique_ptr<TCPClient> client);

		void send(const std::string& packet) override;

	protected:
		void on_recv(std::vector<uint8_t> data) override;

	private:
		PacketAcumulator acumulator;
	};

	class WorldSession : public Session {
	public:
		explicit WorldSession(std::unique_ptr<TCPClient> client, uint32_t session_id);

		void send(const std::string& packet) override;

	protected:
		void on_recv(std::vector<uint8_t> data) override;
	private:
		PacketAcumulator acumulator;
		bool is_first_packet = true;
		uint32_t session_id;
	};


}