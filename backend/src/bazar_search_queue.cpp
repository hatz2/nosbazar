#include "bazar_search_queue.h"

void nosbazar::BazarSearchQueue::push(std::shared_ptr<BazarSearch> search)
{
	const std::lock_guard<std::mutex> lock(mutex);
	queue.push(std::move(search));
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
