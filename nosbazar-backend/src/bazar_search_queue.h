#pragma once

#include <future>
#include <mutex>
#include <queue>

namespace nosbazar {
	using GameServer = int;

	struct BazarRequest {
		// TODO: Define fields
		std::string packet;
	};

	struct BazarResponse {
		// TODO: Define fields
		std::promise<std::string> packet;
	};

	struct BazarSearch {
		BazarRequest request;
		BazarResponse response;
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

		std::unordered_map<GameServer, BazarSearchQueue> server_queues;
	};
}