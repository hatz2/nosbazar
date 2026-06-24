# AGENTS.md — nosbazar

## Repo layout (monorepo)

| Directory | Tech | Purpose |
|-----------|------|---------|
| `nosbazar-backend/` | C++23 / CMake | Game server bot: clientless Nostale login, bazaar search, HTTP API on `:8080` |
| `frontend/` | SvelteKit + TS | Web UI (search interface for the backend) |
| `test/` | Node.js | E2E / integration tests (minimal; check structure before relying) |

---

## Backend build system

- **CMake presets** in `nosbazar-backend/CMakePresets.json`. Use preset-driven commands, never raw `cmake ..`.
- **vcpkg manifest mode** — dependencies declared in `vcpkg.json` are fetched automatically. No manual install needed on first configure.
- Build from the root of `nosbazar-backend/`:

```bash
# Configure (pick a preset that matches your platform)
cmake --preset x64-debug           # Windows MSVC + Ninja
cmake --preset linux-debug         # Linux GCC/Clang + Ninja

# Build
cmake --build --preset x64-debug   # or linux-debug

# The binary lands in out/build/<preset>/.  Assets and .env are copied there post-build.
```

- No linter, formatter, typecheck, or test framework is configured for the backend. Treat `main.cpp` + compile-as-verify.

---

## Backend entrypoint & runtime flow

1. `src/main.cpp` starts a Crow HTTP server on port **8080**.
2. A detached thread runs `Clientless` — logs into Nostale's login server, then a world server, subscribing to bazaar list packets.
3. GET `/search` accepts JSON with item filters, queues a `BazarSearchTask`, and blocks until the game-server response arrives via future/promise in the packet publisher.
4. The hard-coded path `C:/Program Files (x86)/Nostale/NostaleData/NSgtdData.NOS` is used to bootstrap item metadata parsing. On Linux or different installs, this likely needs adjustment. Check `io/nos_file_text_reader.h` / `io/item_dat_parser.cpp`.

---

## Key source structure (backend)

| Directory/files | Role |
|-----------------|------|
| `auth/`         | Login protocol: identity, token repo, session management |
| `packets/`     | Game packet definitions and the publisher/subscriber dispatcher |
| `agent/`       | In-game agent: state machine (walk, think, open bazaar, search), pathfinding via third-party A* |
| `io/`          | Decryptors & parsers for `.NOS` archive files, item data, language tables |
| `clientless.h/cpp` | High-level orchestrator — login flow → packet subscription → request dispatch |
| `env.h`        | Reads configuration from `.env` via a custom dotenv loader (`dotenv.h`) |

Third-party code lives in `third_party/astar/`.

---

## Environment & secrets

- `.env` at the backend root provides game credentials and server config. It is **gitignored** (`*.env` in `.gitignore`). The build copies it into the output directory via a CMake post-build step.
- `IDENTITY_PATH` points to a local JSON file holding persistent login state.
- Game data archive (`NostaleData`) is also gitignored. Ensure it exists at the expected path or update the code before running.

---

## Frontend

SvelteKit project in `frontend/`. Standard SvelteKit commands apply:

```bash
cd frontend
npm install
npm run dev          # dev server (default :5173 via Vite)
npm run build        # production build
npm run check        # TypeScript typecheck (if available, verify package.json scripts)
```

Linting via `eslint.config.js`, formatting via `.prettierrc`. Run lint/format before committing.

---

## Things agents get wrong

- **Do not add a test framework to the backend** — none exists and there is no convention to follow. If tests are needed, propose a design first.
- **Do not remove the manual source list** from `CMakeLists.txt` line 31 without verifying file-glob alternatives. It's intentional for this project size.
- **Never commit `.env` or game data files** — both are gitignored by pattern.
- The Crow route handler blocks on `task->response.packet.get_future().get()` — synchronous blocking is by design, not a bug to "fix".
- **Do not try to compile the project** - but still try to detect compilation errors before submitting a response
