# AGENTS.md — nosbazar

## Repo layout (monorepo)

| Directory | Tech | Purpose |
|-----------|------|---------|
| `backend/` | C++23 / CMake | Game server bot: clientless Nostale login, bazaar search, HTTP API on `:8080` |
| `frontend/` | SvelteKit + TS | Web UI (search interface for the backend) |

---

## Backend build system

- **CMake presets** in `backend/CMakePresets.json`. Use preset-driven commands, never raw `cmake ..`.
- **vcpkg manifest mode** — dependencies declared in `vcpkg.json` are fetched automatically. No manual install needed on first configure.
- Build from the root of `backend/`:

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
4. `src/main.cpp` calls `nosclient::check_and_download_outdated_files()` on startup, which downloads missing or outdated game data (`NSgtdData.NOS`, `NSipData.NOS`, language files, client exes) from Gameforge patch servers into `assets/NostaleData/` (relative to the working directory), verifying SHA1 against the remote manifest. Item metadata is then parsed from `assets/NostaleData/NSgtdData.NOS` (see `io/nos_file_text_reader.h` / `io/item_dat_parser.cpp`). Internet access is required on first run.

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
- Game data files (`assets/NostaleData/`) are downloaded automatically on startup and are gitignored. Run the binary from its build output directory so the relative path resolves.

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
- **When committing, do not modify any existing files** — stage and commit only files that are already dirty. Never edit, reformat, or change code during a commit.
- **Search before building** — always grep/glob for existing utilities, helpers, components, types, or patterns before writing new code. If similar functionality exists, extend it instead of duplicating.
- **Keep all comments and documentation in English** — including markdown files, code comments, and commit messages.
- **Never commit unless explicitly told to** — wait for the user to say "commit" or similar before staging any changes.

---

## Git commit conventions

- **Use conventional commits** — follow the format: `<type>(scope): description`.
- **Types**: `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`, `revert`.
- **Scope**: the area being changed (e.g., `backend`, `frontend`, `format`, `agent`).
- **Description**: imperative mood, lowercase, no period at the end (e.g., `feat(frontend): add shell effect formatting`).
- **Multi-line bodies**: add a blank line after the subject, then the body for context. Keep body wrapped at 72 characters.
- **Breaking changes**: include a `!` before `:` and add a `BREAKING CHANGE:` footer if the change is not backward compatible (e.g., `feat!: rename search endpoint`).

---

## Shell effects on weapons/armor (applied shells)

- `WeaponData` and `ArmourData` carry a `shells: ShellEffect[]` array serialized by the backend.
- Each `ShellEffect` has `{grade, vnum, value, upgrade}`.
- `upgrade` encodes both level and "last upgraded" via its sign:
  - `0` = no upgrade
  - `>0` = has upgrade
  - `<0` = was the **last** shell option upgraded (wraps output in `[ ... ]`)
- `formatAppliedShellEffectString` in `format.ts` handles display: adds `++` prefix and `(+bonus)` suffix.
- Base upgrade values per vnum are in `WeaponShellUpgradeBaseValue` / `ArmorShellUpgradeBaseValue` (`format.ts`). Default fallback is `10` for unknown vnums.

---

## C++ conventions

- **Headers**: use `#pragma once`, not include guards. Keep headers self-contained — include what you use.
- **Naming**: `snake_case` for functions and variables, `PascalCase` for types and enums. Member variables use `snake_case` with no prefix.
- **std::variant dispatch**: use `std::visit` with a lambda, not manual `if-holds-alternative` chains. See existing pattern in `rc_blist.cpp` `json()`.
- **String handling**: prefer `std::string_view` for parameters and parsing tokens. Avoid `std::string` ownership where not needed.
- **No raw loops over packets** — use the token-based parser pattern in `strings::parse.h` instead of manual string splitting.
- **No exceptions in hot paths** — game packet parsing runs per-tick; use `std::optional` or error codes for expected failures.
- **Remove unused `#include`** — before committing, check that every `#include` is used. If a header is no longer needed, remove it.

---

## Svelte / TypeScript conventions

- **Svelte 5 runes**: use `$props()`, `$state()`, `$derived()`, `$effect()`. Do not use `export let`, `let:` directives, or `$:` reactive statements.
- **Typing**: always type component props via a `type Props = { ... }` interface. Avoid `any` — use proper discriminated unions and type guards.
- **Imports**: group by: (1) Svelte internals, (2) project utils/services, (3) types. Use `$lib/` path alias, never relative imports that traverse up.
- **DOM access**: never use `document.querySelector` or direct DOM manipulation in components. Use Svelte bindings or `getElementById` only as last resort.
- **Store access**: use `import { lang } from '$lib/stores/lang.svelte'` with `$derived` / rune syntax. Do not use Svelte's legacy `$store` prefix syntax.
- **No inline styles in templates** — define classes in `<style>` block. Exception: dynamic colors (e.g., `ShellGradeColor`).
- **Remove unused imports** — before committing, check that every `import` is used. If an import is no longer needed, remove it (both TS imports and Svelte component imports).
