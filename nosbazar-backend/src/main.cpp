#include "net.h"
#include <spdlog/spdlog.h>
#include "auth/nosauth.h"
#include "dotenv.h"
#include <asio.hpp>
#include <random/random.h>
#include <login_packet.h>
#include <strings/hex.h>
#include <nosclient.h>
#include "env.h"
#include <auth/token_repository.h>
#include <algorithm>
#include <time/interval_timer.h>
#include "clientless.h"

int main(int argc, char** argv) {
#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#else
	spdlog::set_level(spdlog::level::info);
#endif

	Env env;
	nosbazar::Clientless client(env.account_id, env.world_server_id, env.world_server_channel);
	client.run();
}

