#include "agent.h"
#include "walk_state.h"
#include "walk_think_state.h"
#include "open_bazar_state.h"
#include "search_bazar_state.h"

nosbazar::agent::Agent::Agent(net::WorldSession& session, packets::Publisher& publisher, GameServer server)
{
	std::vector<std::shared_ptr<State>> states = {
		std::make_shared<WalkState>(session),
		std::make_shared<BazarPortalEnterState>(session),
		std::make_shared<BazarNpcWalkState>(),
		std::make_shared<BazarPortalWalkState>(),
		std::make_shared<OpenBazarState>(session),
		std::make_shared<SearchBazarState>(session, publisher, server)
	};

	state_machine = std::make_unique<StateMachine>(states);
}

void nosbazar::agent::Agent::run(const game::Sensors& sensors)
{
	state_machine->act(sensors);
}
