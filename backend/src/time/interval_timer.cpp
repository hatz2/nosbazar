#include "interval_timer.h"
#include <spdlog/spdlog.h>
#include <system_error>
#include <utility>

nosbazar::time::IntervalTimer::State::State(asio::io_context& io, asio::steady_timer::duration interval, std::function<void()> callback)
	: timer(io)
	, interval(interval)
	, callback(std::move(callback))
{
}

nosbazar::time::IntervalTimer::IntervalTimer(asio::io_context& io, asio::steady_timer::duration interval, std::function<void()> callback)
	: state(std::make_shared<State>(io, interval, std::move(callback)))
{
}

nosbazar::time::IntervalTimer::~IntervalTimer()
{
	stop();
}

void nosbazar::time::IntervalTimer::start()
{
	if (state->active) {
		return;
	}
	state->active = true;
	++state->generation;
	schedule(state);
}

void nosbazar::time::IntervalTimer::stop() noexcept
{
	if (!state->active) {
		return;
	}
	state->active = false;
	try {
		state->timer.cancel();
	} catch (const std::system_error&) {
		// The inactive state still prevents pending handlers from invoking callbacks.
	}
}

void nosbazar::time::IntervalTimer::schedule(const std::shared_ptr<State>& state)
{
	const auto generation = state->generation;
	try {
		state->timer.expires_after(state->interval);
		// Pending handlers own the state, never the IntervalTimer wrapper.
		state->timer.async_wait([state, generation](const asio::error_code& error) {
			if (!state->active || generation != state->generation) {
				return;
			}
			if (error) {
				state->active = false;
				if (error != asio::error::operation_aborted) {
					SPDLOG_ERROR("Interval timer failed: {}", error.message());
				}
				return;
			}

			try {
				state->callback();
			} catch (...) {
				if (generation == state->generation) {
					state->active = false;
				}
				throw;
			}
			if (state->active && generation == state->generation) {
				schedule(state);
			}
		});
	} catch (...) {
		state->active = false;
		throw;
	}
}
