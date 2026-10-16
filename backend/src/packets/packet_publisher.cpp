#include "packet_publisher.h"
#include <algorithm>
#include <utility>
#include <strings/parse.h>

nosbazar::packets::Publisher::Subscription::Subscription(Publisher& publisher, SubscriptionId id) noexcept
	: publisher(&publisher), id(id)
{
}

nosbazar::packets::Publisher::Subscription::Subscription(Subscription&& other) noexcept
	: publisher(std::exchange(other.publisher, nullptr)), id(std::exchange(other.id, 0))
{
}

nosbazar::packets::Publisher::Subscription& nosbazar::packets::Publisher::Subscription::operator=(Subscription&& other) noexcept
{
	if (this != &other) {
		reset();
		publisher = std::exchange(other.publisher, nullptr);
		id = std::exchange(other.id, 0);
	}
	return *this;
}

nosbazar::packets::Publisher::Subscription::~Subscription()
{
	reset();
}

void nosbazar::packets::Publisher::Subscription::reset() noexcept
{
	auto* owner = std::exchange(publisher, nullptr);
	const auto subscription_id = std::exchange(id, 0);
	if (owner != nullptr) {
		owner->unsubscribe(subscription_id);
	}
}

nosbazar::packets::Publisher::SubscriptionId nosbazar::packets::Publisher::subscribe(std::string_view opcode, Callback callback)
{
	const auto id = next_subscription_id++;
	listeners[std::string(opcode)].push_back(std::make_shared<Listener>(Listener{id, std::move(callback)}));
	return id;
}

nosbazar::packets::Publisher::Subscription nosbazar::packets::Publisher::subscribe_scoped(std::string_view opcode, Callback callback)
{
	return Subscription(*this, subscribe(opcode, std::move(callback)));
}

void nosbazar::packets::Publisher::unsubscribe(SubscriptionId id) noexcept
{
	for (auto it = listeners.begin(); it != listeners.end(); ++it) {
		auto& entries = it->second;
		const auto listener = std::find_if(entries.begin(), entries.end(), [id](const auto& entry) {
			return entry->id == id;
		});
		if (listener != entries.end()) {
			(*listener)->active = false;
			entries.erase(listener);
			if (entries.empty()) {
				listeners.erase(it);
			}
			return;
		}
	}
}

void nosbazar::packets::Publisher::publish(const std::string& packet) const
{
	std::string_view packet_view = packet;
	std::string_view header = strings::token<std::string_view>(packet_view, ' ');

	const auto it = listeners.find(std::string(header));
	if (it == listeners.end()) {
		return;
	}

	// Keep callbacks alive during dispatch, even if they unsubscribe themselves.
	// New subscriptions are deferred to the next publish; removed ones are skipped.
	const auto snapshot = it->second;
	for (const auto& listener : snapshot) {
		if (listener->active) {
			listener->callback(packet);
		}
	}
}
