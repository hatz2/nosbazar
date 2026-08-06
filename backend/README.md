# NosBazar backend

Clientless Nostale client and bazaar search service written in C++23.
It logs into the game's login and world servers, subscribes to bazaar
packets and exposes an HTTP API on port `8080`.

## Requirements

- C++23 compiler: MSVC (Windows) or GCC/Clang (Linux/macOS)
- CMake 3.21+ (JSON preset support, schema version 3)
- Ninja build system
- [vcpkg](https://github.com/microsoft/vcpkg) with `VCPKG_ROOT` set —
  dependencies from `vcpkg.json` are fetched automatically (manifest mode)
- Internet access on first run (game data files are downloaded below)

## Build

Configure and build with a preset matching your platform:

```sh
# Windows (MSVC + Ninja) — run from an "x64 Native Tools Command
# Prompt" so the MSVC environment (cl.exe) is available
cmake --preset x64-debug     # or x64-release

# Linux (GCC/Clang + Ninja)
cmake --preset linux-debug

# macOS (Clang + Ninja)
cmake --preset macos-debug
```

The Windows presets wire `CMAKE_TOOLCHAIN_FILE` to
`$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake`, so `VCPKG_ROOT` must
point at your vcpkg checkout. On Linux/macOS the presets do not set a
toolchain; to use the manifest-mode dependencies from `vcpkg.json`, make
vcpkg available (e.g. pass
`-DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake` or
run `vcpkg integrate install`).

Then build:

```sh
cmake --build --preset x64-release   # or linux-debug
```

The `backend` binary lands in `out/build/<preset>/`. The `assets/`
directory and the `.env` file are copied into the output directory by a
post-build step.

## Configuration

Create a `.env` file in the `backend/` directory with:

| Variable           | Description                             |
|--------------------|-----------------------------------------|
| `IDENTITY_PATH`    | Path to the JSON file holding login state |
| `GF_EMAIL`         | Gameforge account email                 |
| `GF_PASSWORD`      | Gameforge account password              |
| `INSTALLATION_ID`  | Client installation id                  |
| `LOGIN_SERVER_IP`  | Nostale login server address            |
| `LOGIN_SERVER_PORT`| Nostale login server port               |
| `MAX_CLIENTS`      | Maximum concurrent clientless sessions  |

The `.env` file is gitignored — never commit it.

## Game data

On startup the backend compares SHA1 hashes against the Gameforge patch
manifest and downloads any missing or outdated game files — `NSgtdData.NOS`,
`NSipData.NOS`, the language files and client executables — into
`assets/NostaleData/` (relative to the working directory) from
`http://patches.gameforge.com/`. Internet access is required on first run.

Run the binary from its build output directory so the relative
`assets/NostaleData` path resolves.

## Run

Start the built binary from its output directory:

```sh
./backend   # or backend.exe on Windows
```

The HTTP API listens on port `8080`:

| Endpoint                 | Method | Description                  |
|--------------------------|--------|------------------------------|
| `/search`                | POST   | Search the bazaar            |
| `/servers`               | GET    | List available servers       |
| `/items/static/<id>`     | GET    | Item static data             |
| `/monster/static/<id>`   | GET    | Monster static data          |
| `/skill/static/<id>`     | GET    | Skill static data            |
| `/icon/<id>`             | GET    | Item icon image              |
| `/conststring/<id>`      | GET    | Localized const string       |
| `/bcard/string`          | GET    | Buff card localized string   |

## Legal

NosBazar is an unofficial, non-commercial fan project. All game content
belongs to Gameforge 4D GmbH and Entwell Co., Ltd. This project is not
affiliated with or endorsed by Gameforge or Entwell. Using a clientless
login may violate the game's terms of service; use at your own risk.