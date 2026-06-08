#pragma once

#include <asio.hpp>
#include <chrono>
#include <functional>

namespace nosbazar::time {
	class IntervalTimer {
	public:
		IntervalTimer(asio::io_context& io, asio::steady_timer::duration interval, std::function<void()> callback);

		void start();

	private:

		void schedule();

		asio::steady_timer timer;
		asio::steady_timer::duration interval;
		std::function<void()> callback;
	};
}