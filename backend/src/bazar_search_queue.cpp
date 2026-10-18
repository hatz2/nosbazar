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

bool nosbazar::BazarSearch::is_pending()
{
	const std::lock_guard<std::mutex> lock(mutex);
	expire_locked(Clock::now());
	return !completed && !dispatched;
}

bool nosbazar::BazarSearch::is_completed()
{
	const std::lock_guard<std::mutex> lock(mutex);
	expire_locked(Clock::now());
	return completed;
}

nosbazar::BazarSearchQueue::BazarSearchQueue(std::mutex& mutex, std::size_t& pending_total)
	: mutex(mutex), pending_total(pending_total)
{
}

nosbazar::BazarSearchQueue::~BazarSearchQueue()
{
	set_available(false);
}

void nosbazar::BazarSearchQueue::prune_locked()
{
	pending_total -= std::erase_if(queue, [](const auto& search) { return !search->is_pending(); });
}

std::expected<void, nosbazar::BazarSearchError> nosbazar::BazarSearchQueue::push_locked(std::shared_ptr<BazarSearch> search)
{
	if (!available) {
		return std::unexpected(BazarSearchError::unavailable);
	}
	if (!search->is_pending()) {
		return std::unexpected(BazarSearchError::queue_timeout);
	}
	if (queue.size() >= max_pending_per_server || pending_total >= max_pending_global) {
		return std::unexpected(BazarSearchError::queue_full);
	}
	queue.push_back(std::move(search));
	++pending_total;
	return {};
}

void nosbazar::BazarSearchQueue::set_available(bool value)
{
	std::deque<std::shared_ptr<BazarSearch>> pending;
	{
		const std::lock_guard<std::mutex> lock(mutex);
		available = value;
		if (!available) {
			pending_total -= queue.size();
			queue.swap(pending);
		}
	}
	for (const auto& search : pending) {
		search->fail(BazarSearchError::unavailable);
	}
}

std::shared_ptr<nosbazar::BazarSearch> nosbazar::BazarSearchQueue::pop()
{
	const std::lock_guard<std::mutex> lock(mutex);
	while (!queue.empty()) {
		std::shared_ptr<BazarSearch> search = std::move(queue.front());
		queue.pop_front();
		--pending_total;
		if (search->start()) {
			return search;
		}
	}
	return nullptr;
}

bool nosbazar::BazarSearchQueue::empty()
{
	const std::lock_guard<std::mutex> lock(mutex);
	prune_locked();
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
	return manager.server_queues.try_emplace(server, manager.mutex, manager.pending_total).first->second;
}

std::expected<void, nosbazar::BazarSearchError> nosbazar::BazarSearchManager::submit(GameServer server, std::shared_ptr<BazarSearch> search)
{
	auto& manager = instance();
	const std::lock_guard<std::mutex> lock(manager.mutex);
	if (manager.stopping) {
		return std::unexpected(BazarSearchError::service_stopping);
	}
	const auto it = manager.server_queues.find(server);
	if (it == manager.server_queues.end()) {
		return std::unexpected(BazarSearchError::unavailable);
	}
	// One shared lock covers pruning, both limits, and admission.
	for (auto& [id, queue] : manager.server_queues) {
		queue.prune_locked();
	}
	std::erase_if(manager.searches, [](const auto& task) { return task->is_completed(); });
	// Retain accepted tasks after dequeueing so shutdown also releases active waits.
	manager.searches.push_back(search);
	auto admission = it->second.push_locked(std::move(search));
	if (!admission) {
		manager.searches.pop_back();
	}
	return admission;
}

void nosbazar::BazarSearchManager::shutdown()
{
	auto& manager = instance();
	std::vector<std::shared_ptr<BazarSearch>> searches;
	{
		const std::lock_guard<std::mutex> lock(manager.mutex);
		if (manager.stopping) {
			return;
		}
		manager.stopping = true;
		searches.swap(manager.searches);
		for (auto& [server, queue] : manager.server_queues) {
			queue.available = false;
			queue.queue.clear();
		}
		manager.pending_total = 0;
	}
	for (const auto& search : searches) {
		search->fail(BazarSearchError::service_stopping);
	}
}
