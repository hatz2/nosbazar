#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>

namespace nosbazar::packets {
	class Publisher {
	public:
		Publisher() = default;
		Publisher(const Publisher&) = delete;
		Publisher& operator=(const Publisher&) = delete;

		using Callback = std::function<void(const std::string&)>;
		using SubscriptionId = std::uint64_t;

		// Subscription changes and dispatch must run on the owning client thread.
		SubscriptionId subscribe(std::string_view opcode, Callback callback);
		void unsubscribe(SubscriptionId id);

		void publish(const std::string& packet) const;

	private:
		struct Listener {
			SubscriptionId id;
			Callback callback;
			bool active{true};
		};

		SubscriptionId next_subscription_id{1};
		std::unordered_map<std::string, std::vector<std::shared_ptr<Listener>>> listeners;
	};
}
