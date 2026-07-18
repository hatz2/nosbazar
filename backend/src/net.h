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
#include <packets/packet_publisher.h>

namespace nosbazar::net {
	struct Response {
		int status_code;
		std::string body;
		std::unordered_map<std::string, std::string> headers;
	};

	enum class CurlError {
		curl_init,
		curl_easy_perform,
	};

	std::expected<Response, CurlError> post(std::string_view url, std::string_view body, const std::vector<std::string>& headers = {});

	std::expected<Response, CurlError> get(std::string_view url, const std::vector<std::string>& headers = {});

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
		explicit Session(std::unique_ptr<TCPClient> client, packets::Publisher& publisher);

		virtual void send(const std::string& packet) = 0;

	protected:
		void on_connect();
		void on_disconnect();
		virtual void on_recv(std::vector<uint8_t> data) = 0;
		void on_packet(const std::string& packet);
		void on_send(const std::string& packet);

		std::unique_ptr<TCPClient> client;
		std::queue<std::vector<uint8_t>> pending_packets;

	private:
		packets::Publisher& publisher;
		TCPClient::Observer observer;
	};

	class LoginSession : public Session {
	public:
		explicit LoginSession(std::unique_ptr<TCPClient> client, packets::Publisher& publisher);

		void send(const std::string& packet) override;

	protected:
		void on_recv(std::vector<uint8_t> data) override;

	private:
		PacketAcumulator acumulator;
	};

	class WorldSession : public Session {
	public:
		explicit WorldSession(std::unique_ptr<TCPClient> client, packets::Publisher& publisher, uint16_t session_id);

		void send(const std::string& packet) override;

	protected:
		void on_recv(std::vector<uint8_t> data) override;
	private:
		
		PacketAcumulator acumulator;
		uint16_t session_id;
		uint16_t packet_counter;
		bool first_packet_sent{false};
	};


}