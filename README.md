# NosBazar

Online bazaar search for the MMORPG NosTale. NosBazar runs a clientless
game login in the background and exposes a searchable bazaar API with a
web frontend.

> **Disclaimer:** All game content, images, trademarks and data belong to
> Gameforge 4D GmbH and Entwell Co., Ltd. NosBazar is an unofficial,
> non-commercial fan project and is not affiliated with or endorsed by
> Gameforge or Entwell. Using a clientless login may violate the game's
> terms of service; use this project at your own risk.

## Features

- Clientless game client: logs into the Nostale login server and a world
  server without a running game client.
- Bazaar search across multiple servers with filters for item type,
  upgrade, shell effects, level, rarity and runes.
- Rich item details: stats, applied shells, cellons, runes and more.
- Multi-language support (ES, UK, FR, DE, TR, IT, PL, CZ) and
  multi-server support.
- HTTP API consumed by a SvelteKit web frontend.

## Architecture

| Directory   | Tech           | Purpose                                             |
|-------------|----------------|-----------------------------------------------------|
| `backend/`  | C++23 / CMake  | Clientless game server bot, packet parsing, HTTP API |
| `frontend/` | SvelteKit + TS | Web UI for search                                    |

At startup the backend logs into Nostale and subscribes to bazaar list
packets. `POST /search` queues a search task and waits for the game server
response, which is published back over HTTP.

## How to run locally

Run the backend first, then the frontend.

1. **Backend** — configure and build the C++23 service, provide a `.env`
   with game credentials, then start the binary. On first run it
   downloads the game data files automatically and exposes the API on
   `:8080`.
   See [backend/README.md](backend/README.md) for the full steps.
2. **Frontend** — install dependencies and start the dev server; it talks
   to the backend on `localhost:8080`. See
   [frontend/README.md](frontend/README.md) for the full steps.

## Contributing

Bug reports and contributions are welcome via GitHub issues and pull
requests against this repository.

## License

MIT, see [LICENSE](LICENSE). Copyright (c) 2026 NosBazar contributors.

## Legal

NosBazar is an unofficial fan project. It is not affiliated with, endorsed
by, or sponsored by Gameforge 4D GmbH or Entwell Co., Ltd. All game content,
graphics and trademarks are the property of their respective owners.