#include "packet_publisher.h"
#include <sstream>
#include <strings/parse.h>

void nosbazar::packets::Publisher::subscribe(const std::string_view& opcode, Callback callback)
{
	listeners[opcode].push_back(std::move(callback));
}

void nosbazar::packets::Publisher::publish(const std::string& packet) const
{
	std::string_view packet_view = packet;
	std::string_view header = strings::token<std::string_view>(packet_view, ' ');

	if (!listeners.contains(header)) {
		return;
	}

	for (auto& callback : listeners.at(header)) {
		callback(packet);
	}
}
