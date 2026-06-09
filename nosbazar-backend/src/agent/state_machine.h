#pragma once

#include <game/sensors.h>
#include <vector>
#include <ctime>
#include <chrono>

namespace nosbazar::agent {
	class State {
	public:
		virtual bool should_execute(const game::Sensors& sensors) = 0;
		virtual void act(const game::Sensors& sensors) = 0;
	};

	class TimedState : public State {
	public:
		explicit TimedState(const std::pair<float, float>& interval_seconds);
		virtual bool should_execute(const game::Sensors& sensors);

	private:
		void update_interval_time();

		std::pair<float, float> interval_seconds{};
		float interval_time{};
		std::chrono::steady_clock::time_point start_time{};
	};

	class StateMachine {
	public:
		explicit StateMachine(const std::vector<State>& states);
		bool act(const game::Sensors& sensors);

	private:
		std::vector<State> states;
	};

	class StateMachineDirector {
	public:
		explicit StateMachineDirector(const std::vector<StateMachine>& machines);
		void run(const game::Sensors& sensors);

	private:
		std::vector<StateMachine> machines;
	};
}