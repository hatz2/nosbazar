#include "env.h"
#include "clientless.h"
#include <spdlog/spdlog.h>
#include "map_grid_repository.h"

int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	nosbazar::MapGridRepository::instance();

	Env env;
	nosbazar::Clientless client(env.account_id, env.world_server_id, env.world_server_channel);
	return static_cast<int>(client.run());
}

