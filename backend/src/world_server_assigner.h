#pragma once

#include <atomic>
#include <vector>
#include "packets/login_packet.h"

namespace nosbazar {
	class WorldServerAssigner {
	public:
		static WorldServerAssigner& instance() {
			static WorldServerAssigner inst;
			return inst;
		}

		const packets::login::WorldServer& assign(
			const std::vector<packets::login::WorldServer>& servers)
		{
			size_t idx = next_server.fetch_add(1, std::memory_order_relaxed);
			return servers[idx % servers.size()];
		}

	private:
		WorldServerAssigner() = default;
		~WorldServerAssigner() = default;
		WorldServerAssigner(const WorldServerAssigner&) = delete;
		WorldServerAssigner& operator=(const WorldServerAssigner&) = delete;

		std::atomic<size_t> next_server{0};
	};
}
