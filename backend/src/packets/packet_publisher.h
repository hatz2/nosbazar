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

		// The publisher must outlive this handle. All operations use the client thread.
		class Subscription {
		public:
			Subscription() = default;
			Subscription(const Subscription&) = delete;
			Subscription& operator=(const Subscription&) = delete;
			Subscription(Subscription&& other) noexcept;
			Subscription& operator=(Subscription&& other) noexcept;
			~Subscription();
			void reset() noexcept;

		private:
			friend class Publisher;
			Subscription(Publisher& publisher, SubscriptionId id) noexcept;
			Publisher* publisher{};
			SubscriptionId id{};
		};

		// Subscription changes and dispatch must run on the owning client thread.
		SubscriptionId subscribe(std::string_view opcode, Callback callback);
		[[nodiscard]] Subscription subscribe_scoped(std::string_view opcode, Callback callback);
		void unsubscribe(SubscriptionId id) noexcept;

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
