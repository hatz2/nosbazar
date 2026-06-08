#include "interval_timer.h"
#include <spdlog/spdlog.h>

nosbazar::time::IntervalTimer::IntervalTimer(asio::io_context& io, asio::steady_timer::duration interval, std::function<void()> callback)
	: timer(io)
	, interval(interval)
	, callback(std::move(callback))
{
}

void nosbazar::time::IntervalTimer::start()
{
	schedule();
}

void nosbazar::time::IntervalTimer::schedule()
{
	timer.expires_after(interval);
	timer.async_wait([this](std::error_code ec) {
		if (ec) {
			SPDLOG_ERROR(ec.message());
			return;
		}

		callback();
		schedule();
	});
}
