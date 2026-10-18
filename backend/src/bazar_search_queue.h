#pragma once

#include <future>
#include <chrono>
#include <expected>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <packets/c_blist.h>

namespace nosbazar {
	using GameServer = int;

	struct BazarRequest {
		// Actual fields
		packets::CBlist search_packet;
	};

	enum class BazarSearchError {
		disconnected,
		invalid_response,
		unavailable,
		queue_timeout,
		response_timeout
	};

	using BazarSearchResult = std::expected<std::string, BazarSearchError>;

	class BazarSearch {
	public:
		BazarRequest request;
		static constexpr auto queue_timeout = std::chrono::seconds(30);
		static constexpr auto response_timeout = std::chrono::seconds(10);

		// Retrieve once, before enqueueing. Only the first completion succeeds.
		std::future<BazarSearchResult> get_future();
		bool succeed(std::string response);
		bool fail(BazarSearchError error);
		// Claim queued work and start its response deadline; expired work is rejected.
		bool start();
		void expire();
		bool response_expired();

	private:
		using Clock = std::chrono::steady_clock;
		bool complete(BazarSearchResult result);
		bool complete_locked(BazarSearchResult result);
		bool expire_locked(Clock::time_point now);
		std::mutex mutex;
		std::promise<BazarSearchResult> promise;
		bool completed{false};
		bool dispatched{false};
		bool response_timed_out{false};
		Clock::time_point deadline{Clock::now() + queue_timeout};
	};

	class BazarSearchQueue {
	public:
		bool push(std::shared_ptr<BazarSearch> search);
		std::shared_ptr<BazarSearch> pop();
		bool empty();
		void set_available(bool available);

	private:
		std::mutex mutex;
		std::queue<std::shared_ptr<BazarSearch>> queue;
		bool available{false};
	};

	class BazarSearchManager {
	public:
		static BazarSearchQueue& queue(const GameServer server);
		static bool submit(GameServer server, std::shared_ptr<BazarSearch> search);

	private:
		static BazarSearchManager& instance();

		BazarSearchManager() = default;
		BazarSearchManager(const BazarSearchManager&) = delete;
		void operator=(const BazarSearchManager&) = delete;

		std::mutex mutex;
		std::map<GameServer, BazarSearchQueue> server_queues;
	};
}
