#include "search_bazar_state.h"
#include <spdlog/spdlog.h>
#include <packets/rc_blist.h>

nosbazar::agent::SearchBazarState::SearchBazarState(net::WorldSession& session, packets::Publisher& publisher, GameServer server) : session(session), server(server)
{
	publisher.subscribe("rc_blist", [this](auto& packet) { on_rc_blist(packet); });
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

	// Send back the resposne
	std::string response(packet);

	packets::RcBlist rc_packet(packet);

	nlohmann::json json_response = rc_packet.json();

	std::string result = json_response.dump(4);

	task->response.packet.set_value(result);

	// Mark task as completed for us
	task.reset();
}
