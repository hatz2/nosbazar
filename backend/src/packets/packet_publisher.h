#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

namespace nosbazar::packets {
	class Publisher {
	public:
		using Callback = std::function<void(const std::string&)>;

		void subscribe(const std::string_view& opcode, Callback callback);

		void publish(const std::string& packet) const;

	private:
		std::unordered_map<std::string_view, std::vector<Callback>> listeners;
	};
}