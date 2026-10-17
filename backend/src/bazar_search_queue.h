#pragma once

#include <future>
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
		invalid_response
	};

	using BazarSearchResult = std::expected<std::string, BazarSearchError>;

	class BazarSearch {
	public:
		BazarRequest request;

		// Retrieve once, before enqueueing. Only the first completion succeeds.
		std::future<BazarSearchResult> get_future();
		bool succeed(std::string response);
		bool fail(BazarSearchError error);

	private:
		bool complete(BazarSearchResult result);
		std::mutex mutex;
		std::promise<BazarSearchResult> promise;
		bool completed{false};
	};

	class BazarSearchQueue {
	public:
		void push(std::shared_ptr<BazarSearch> search);
		std::shared_ptr<BazarSearch> pop();
		bool empty();

	private:
		std::mutex mutex;
		std::queue<std::shared_ptr<BazarSearch>> queue;
	};

	class BazarSearchManager {
	public:
		static BazarSearchQueue& queue(const GameServer server);

	private:
		static BazarSearchManager& instance();

		BazarSearchManager() = default;
		BazarSearchManager(const BazarSearchManager&) = delete;
		void operator=(const BazarSearchManager&) = delete;

		std::mutex mutex;
		std::map<GameServer, BazarSearchQueue> server_queues;
	};
}
