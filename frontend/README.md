# NosBazar frontend

SvelteKit web UI for the NosBazar bazaar search. It talks to the NosBazar
backend HTTP API. All API requests are made as same-origin relative URLs under
the `/api` prefix, so no absolute `localhost` host is referenced by the client.

## Requirements

- Node.js 18+ (comes with npm/pnpm/yarn)

## Setup

```sh
npm install
```

## Run the dev server

```sh
npm run dev
```

The app runs at `http://localhost:5173`. During development, Vite proxies
`/api/*` requests to the backend on `localhost:8080`, which must be running for
the search and selectors to work — see the root
[README](../README.md#how-to-run-locally) or
[backend/README.md](../backend/README.md).

> In production (adapter-static), the `/api` prefix must be forwarded to the
> backend by a reverse proxy (e.g. Nginx/caddy), since the Vite proxy only
> applies to the dev server.

## Const string caching

Const string translations are cached in `localStorage` under
`nosbazar-conststrings` so browser reloads reuse already-fetched strings. The
cache is keyed by a SHA1 digest of the game's const string data, served by
`GET /api/conststring/version`; when the game data changes the backend serves
a new digest and the frontend discards the stale cache and re-fetches lazily.

## Checks

```sh
npm run check    # Svelte/TypeScript check
npm run lint     # Prettier + ESLint
```

## Production build

```sh
npm run build
npm run preview  # preview the production build locally
```

> To deploy, you may need to install an [adapter](https://svelte.dev/docs/kit/adapters)
> for your target environment.

## Legal

All game content and images belong to Gameforge 4D GmbH and Entwell
Co., Ltd. NosBazar is an unofficial, non-commercial fan project.