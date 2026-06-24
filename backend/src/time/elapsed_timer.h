#pragma once

#include <chrono>

namespace nosbazar::time {
	class ElapsedTimer {
	public:
		// Returns elapsed time in milliseconds
		int64_t elapsed() const;

		// Start timer and return elapsed time in milliseconds
		int64_t restart();

		// Start the timer
		void start();

	private:
		std::chrono::steady_clock::time_point time{ std::chrono::steady_clock::now() };
	};
}