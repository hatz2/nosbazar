#include "state_machine.h"
#include <random/random.h>

nosbazar::agent::StateMachine::StateMachine(const std::vector<State>& states)
	: states(states)
{

}

bool nosbazar::agent::StateMachine::act(const game::Sensors& sensors)
{
	for (auto& state : states) {
		if (state.should_execute(sensors)) {
			state.act(sensors);
			return true;
		}
	}

	return false;
}

void nosbazar::agent::StateMachineDirector::run(const game::Sensors& sensors)
{
	for (auto& machine : machines) {
		if (machine.act(sensors)) {
			break;
		}
	}
}

nosbazar::agent::TimedState::TimedState(const std::pair<float, float>& interval_seconds)
	: interval_seconds(interval_seconds)
	, interval_time(0.0f)
	, start_time(std::chrono::steady_clock::now())
{
	update_interval_time();
}

bool nosbazar::agent::TimedState::should_execute(const game::Sensors& sensors)
{
	float time_diff = std::chrono::duration<float>(std::chrono::steady_clock::now() - start_time).count();

	if (time_diff > interval_time) {
		start_time = std::chrono::steady_clock::now();
		update_interval_time();
		return true;
	}

	return false;
}

void nosbazar::agent::TimedState::update_interval_time()
{
	interval_time = random::random_real<float>(interval_seconds.first, interval_seconds.second);
}
