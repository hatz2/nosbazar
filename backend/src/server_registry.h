#pragma once

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace nosbazar {
	class ServerRegistry {
	public:
		struct ServerInfo {
			int id;
			std::string name;
		};

		static ServerRegistry& instance() {
			static ServerRegistry inst;
			return inst;
		}

		void register_server(int id, std::string name) {
			std::lock_guard lock(mutex);
			servers[id] = std::move(name);
		}

		void unregister_server(int id) {
			std::lock_guard lock(mutex);
			servers.erase(id);
		}

		std::vector<ServerInfo> get_servers() const {
			std::lock_guard lock(mutex);
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

		mutable std::mutex mutex;
		std::unordered_map<int, std::string> servers;
	};
}
