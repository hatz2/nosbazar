#include "env.h"
#include "clientless.h"
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	Env env;
	nosbazar::Clientless client(env.account_id, env.world_server_id, env.world_server_channel);
	return static_cast<int>(client.run());
}

