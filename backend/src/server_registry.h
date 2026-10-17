#pragma once

#include <mutex>
#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "bazar_search_queue.h"

namespace nosbazar {
	class ServerRegistry {
	public:
		using WorkerId = std::uint64_t;
		struct ServerInfo {
			int id;
			std::string name;
		};

		static ServerRegistry& instance() {
			static ServerRegistry inst;
			return inst;
		}

		WorkerId register_worker(int server, std::string name) {
			// Registry changes serialize queue availability with worker registration.
			std::lock_guard lock(mutex);
			auto& queue = BazarSearchManager::queue(server);
			const auto worker = next_worker_id++;
			workers.emplace(worker, ServerInfo{server, std::move(name)});
			queue.set_available(true);
			return worker;
		}

		void unregister_worker(WorkerId worker) {
			std::lock_guard lock(mutex);
			const auto it = workers.find(worker);
			if (it == workers.end()) {
				return;
			}
			const int server = it->second.id;
			workers.erase(it);
			if (!available_locked(server)) {
				BazarSearchManager::queue(server).set_available(false);
			}
		}

		bool is_available(int server) const {
			std::lock_guard lock(mutex);
			return available_locked(server);
		}

		std::vector<ServerInfo> get_servers() const {
			std::lock_guard lock(mutex);
			std::unordered_map<int, std::string> servers;
			for (const auto& [worker, info] : workers) {
				servers.try_emplace(info.id, info.name);
			}
			std::vector<ServerInfo> result;
			result.reserve(servers.size());
			for (const auto& [id, name] : servers) {
				result.push_back({id, name});
			}
			return result;
		}

	private:
		ServerRegistry() = default;
		~ServerRegistry() = default;
		ServerRegistry(const ServerRegistry&) = delete;
		ServerRegistry& operator=(const ServerRegistry&) = delete;

		bool available_locked(int server) const {
			return std::ranges::any_of(workers, [server](const auto& entry) {
				return entry.second.id == server;
			});
		}

		mutable std::mutex mutex;
		WorkerId next_worker_id{1};
		std::unordered_map<WorkerId, ServerInfo> workers;
	};
}
