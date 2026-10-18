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
	if (expire_locked(Clock::now())) {
		return false;
	}
	return complete_locked(std::move(result));
}

bool nosbazar::BazarSearch::complete_locked(BazarSearchResult result)
{
	if (completed) {
		return false;
	}
	promise.set_value(std::move(result));
	completed = true;
	return true;
}

bool nosbazar::BazarSearch::expire_locked(Clock::time_point now)
{
	if (completed || now < deadline) {
		return false;
	}
	response_timed_out = dispatched;
	return complete_locked(std::unexpected(dispatched ? BazarSearchError::response_timeout : BazarSearchError::queue_timeout));
}

bool nosbazar::BazarSearch::start()
{
	const std::lock_guard<std::mutex> lock(mutex);
	const auto now = Clock::now();
	expire_locked(now);
	if (completed || dispatched) {
		return false;
	}
	dispatched = true;
	deadline = now + response_timeout;
	return true;
}

void nosbazar::BazarSearch::expire()
{
	const std::lock_guard<std::mutex> lock(mutex);
	expire_locked(Clock::now());
}

bool nosbazar::BazarSearch::response_expired()
{
	const std::lock_guard<std::mutex> lock(mutex);
	expire_locked(Clock::now());
	return response_timed_out;
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
	while (!queue.empty()) {
		std::shared_ptr<BazarSearch> search = std::move(queue.front());
		queue.pop();
		if (search->start()) {
			return search;
		}
	}
	return nullptr;
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
