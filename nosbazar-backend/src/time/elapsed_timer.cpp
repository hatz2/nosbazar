#include "elapsed_timer.h"

int64_t nosbazar::time::ElapsedTimer::elapsed() const
{
	auto now = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
	return diff;
}

int64_t nosbazar::time::ElapsedTimer::restart()
{
	auto elapsed_time = elapsed();
	start();
	return elapsed_time;
}

void nosbazar::time::ElapsedTimer::start()
{
	time = std::chrono::steady_clock::now();
}
