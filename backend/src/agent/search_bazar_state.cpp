#include "search_bazar_state.h"
#include <spdlog/spdlog.h>
#include <packets/rc_blist.h>
#include <exception>
#include <charconv>
#include <cstdint>
#include <system_error>
#include <strings/parse.h>

nosbazar::agent::SearchBazarState::SearchBazarState(net::WorldSession& session, packets::Publisher& publisher, GameServer server)
	: session(session), server(server),
	  subscription(publisher.subscribe_scoped("rc_blist", [this](const auto& packet) { on_rc_blist(packet); }))
{
}

nosbazar::agent::SearchBazarState::~SearchBazarState()
{
	subscription.reset();
	if (task) {
		task->fail(BazarSearchError::disconnected);
	}
}

bool nosbazar::agent::SearchBazarState::should_execute(const game::Sensors& sensors)
{
	if (exchange != Exchange::ready || !sensors.is_bazar_opened || timer.elapsed() < search_delay_ms) {
		return false;
	}
	return !BazarSearchManager::queue(server).empty();
}

bool nosbazar::agent::SearchBazarState::needs_open() const
{
	return exchange == Exchange::unopened;
}

bool nosbazar::agent::SearchBazarState::is_initialized() const
{
	return exchange == Exchange::ready || exchange == Exchange::searching;
}

void nosbazar::agent::SearchBazarState::begin_open()
{
	if (!needs_open()) {
		return;
	}
	exchange = Exchange::opening;
	opening_deadline = std::chrono::steady_clock::now() + BazarSearch::response_timeout;
	timer.start();
	session.send(packets::CBlist{}.string());
}

void nosbazar::agent::SearchBazarState::act(const game::Sensors& sensors)
{
	if (exchange != Exchange::ready || !sensors.is_bazar_opened || timer.elapsed() < search_delay_ms) {
		return;
	}

	task = BazarSearchManager::queue(server).pop();
	if (!task) {
		return;
	}

	exchange = Exchange::searching;
	timer.start();

	SPDLOG_TRACE("Sending {}", task->request.search_packet.string());
	
	if (poll()) {
		session.send(task->request.search_packet.string());
	}
}

bool nosbazar::agent::SearchBazarState::poll()
{
	if ((exchange == Exchange::opening && std::chrono::steady_clock::now() >= opening_deadline) ||
		(exchange == Exchange::searching && task->response_expired())) {
		fail_exchange(BazarSearchError::response_timeout);
	}
	return exchange != Exchange::failed;
}

void nosbazar::agent::SearchBazarState::fail_exchange(BazarSearchError error)
{
	if (exchange == Exchange::failed) {
		return;
	}
	exchange = Exchange::failed;
	if (task) {
		task->fail(error);
		task.reset();
	}
	// A timed-out response cannot be correlated with later searches on this socket.
	SPDLOG_WARN("Invalid or expired bazaar exchange on server {}; reconnecting", server);
	session.disconnect();
}

void nosbazar::agent::SearchBazarState::on_rc_blist(std::string_view packet)
{
	if (!poll() || (exchange != Exchange::opening && exchange != Exchange::searching)) {
		return;
	}
	auto tokens = packet;
	strings::token<std::string_view>(tokens);
	const auto page_token = strings::token<std::string_view>(tokens);
	std::uint64_t page{};
	const auto [end, error] = std::from_chars(page_token.data(), page_token.data() + page_token.size(), page);
	const auto expected_page = task ? static_cast<std::uint64_t>(task->request.search_packet.index) : 0;
	if (error != std::errc{} || end != page_token.data() + page_token.size() || page != expected_page) {
		fail_exchange(BazarSearchError::invalid_response);
		return;
	}
	if (exchange == Exchange::opening) {
		// Consume the opening query separately; it never completes a user task.
		exchange = Exchange::ready;
		return;
	}
	if (exchange != Exchange::searching || !task) {
		return;
	}

	SPDLOG_TRACE(packet);

	try {
		packets::RcBlist rc_packet(packet);
		if (!task->succeed(rc_packet.json().dump(4))) {
			fail_exchange(BazarSearchError::response_timeout);
			return;
		}
	} catch (const std::exception& error) {
		SPDLOG_WARN("Failed to decode bazaar response: {}", error.what());
		fail_exchange(BazarSearchError::invalid_response);
		return;
	}

	// Mark task as completed for us
	task.reset();
	exchange = Exchange::ready;
}
