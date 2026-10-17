#include "search_bazar_state.h"
#include <spdlog/spdlog.h>
#include <packets/rc_blist.h>
#include <exception>

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
	if (timer.elapsed() < search_delay_ms) {
		return false;
	}

	if (task) {
		return true;
	}

	BazarSearchQueue& queue = BazarSearchManager::queue(server);

	if (queue.empty()) {
		return false;
	}


	task = queue.pop();
	return true;
}

void nosbazar::agent::SearchBazarState::act(const game::Sensors& sensors)
{
	if (timer.elapsed() < search_delay_ms) {
		return;
	}

	if (!task) {
		return;
	}

	timer.start();

	SPDLOG_TRACE("Sending {}", task->request.search_packet.string());
	
	session.send(task->request.search_packet.string());
}

void nosbazar::agent::SearchBazarState::on_rc_blist(std::string_view packet)
{
	if (!task) {
		return;
	}

	SPDLOG_TRACE(packet);

	try {
		packets::RcBlist rc_packet(packet);
		task->succeed(rc_packet.json().dump(4));
	} catch (const std::exception& error) {
		SPDLOG_WARN("Failed to decode bazaar response: {}", error.what());
		task->fail(BazarSearchError::invalid_response);
	}

	// Mark task as completed for us
	task.reset();
}
