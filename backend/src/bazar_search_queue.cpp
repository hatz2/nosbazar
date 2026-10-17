#include "bazar_search_queue.h"
#include <utility>

std::future<nosbazar::BazarSearchResult> nosbazar::BazarSearch::get_future()
{
	const std::lock_guard<std::mutex> lock(mutex);
	return promise.get_future();
}

bool nosbazar::BazarSearch::succeed(std::string response)
{
	return complete(BazarSearchResult(std::move(response)));
}

bool nosbazar::BazarSearch::fail(BazarSearchError error)
{
	return complete(std::unexpected(error));
}

bool nosbazar::BazarSearch::complete(BazarSearchResult result)
{
	const std::lock_guard<std::mutex> lock(mutex);
	if (completed) {
		return false;
	}
	promise.set_value(std::move(result));
	completed = true;
	return true;
}

bool nosbazar::BazarSearchQueue::push(std::shared_ptr<BazarSearch> search)
{
	// Admission and last-worker removal use the same queue mutex.
	const std::lock_guard<std::mutex> lock(mutex);
	if (!available) {
		return false;
	}
	queue.push(std::move(search));
	return true;
}

void nosbazar::BazarSearchQueue::set_available(bool value)
{
	std::queue<std::shared_ptr<BazarSearch>> pending;
	{
		const std::lock_guard<std::mutex> lock(mutex);
		available = value;
		if (!available) {
			queue.swap(pending);
		}
	}
	while (!pending.empty()) {
		pending.front()->fail(BazarSearchError::unavailable);
		pending.pop();
	}
}

std::shared_ptr<nosbazar::BazarSearch> nosbazar::BazarSearchQueue::pop()
{
	const std::lock_guard<std::mutex> lock(mutex);
	if (queue.empty()) {
		return nullptr;
	}
	else {
		std::shared_ptr<BazarSearch> search = std::move(queue.front());
		queue.pop();
		return search;
	}
}

bool nosbazar::BazarSearchQueue::empty()
{
	const std::lock_guard<std::mutex> lock(mutex);
	return queue.empty();
}

nosbazar::BazarSearchManager& nosbazar::BazarSearchManager::instance()
{
	static BazarSearchManager instance;
	return instance;
}

nosbazar::BazarSearchQueue& nosbazar::BazarSearchManager::queue(const GameServer server)
{
	auto& manager = instance();
	const std::lock_guard<std::mutex> lock(manager.mutex);
	return manager.server_queues[server];
}

bool nosbazar::BazarSearchManager::submit(GameServer server, std::shared_ptr<BazarSearch> search)
{
	auto& manager = instance();
	const std::lock_guard<std::mutex> lock(manager.mutex);
	const auto it = manager.server_queues.find(server);
	return it != manager.server_queues.end() && it->second.push(std::move(search));
}
