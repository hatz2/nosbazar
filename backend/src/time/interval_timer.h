#pragma once

#include <asio.hpp>
#include <cstdint>
#include <functional>
#include <memory>

namespace nosbazar::time {
	class IntervalTimer {
	public:
		// All operations run on the owning io_context thread, or while it is not running.
		// The io_context must outlive the timer and its pending handlers.
		IntervalTimer(asio::io_context& io, asio::steady_timer::duration interval, std::function<void()> callback);
		IntervalTimer(const IntervalTimer&) = delete;
		IntervalTimer& operator=(const IntervalTimer&) = delete;
		~IntervalTimer();

		void start();
		void stop() noexcept;

	private:

		struct State {
			State(asio::io_context& io, asio::steady_timer::duration interval, std::function<void()> callback);
			asio::steady_timer timer;
			asio::steady_timer::duration interval;
			std::function<void()> callback;
			std::uint64_t generation{};
			bool active{};
		};

		static void schedule(const std::shared_ptr<State>& state);
		std::shared_ptr<State> state;
	};
}
