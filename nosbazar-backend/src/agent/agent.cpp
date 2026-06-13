#include "agent.h"
#include "walk_state.h"
#include "walk_think_state.h"

nosbazar::agent::Agent::Agent(net::WorldSession& session)
{
	std::vector<std::shared_ptr<State>> states = {
		std::make_shared<WalkState>(session),
		std::make_shared<BazarPortalEnterState>(session),
		std::make_shared<BazarNpcWalkState>(),
		std::make_shared<BazarPortalWalkState>()
	};

	state_machine = std::make_unique<StateMachine>(states);
}

void nosbazar::agent::Agent::run(const game::Sensors& sensors)
{
	state_machine->act(sensors);
}
