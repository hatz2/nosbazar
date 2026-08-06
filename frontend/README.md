# NosBazar frontend

SvelteKit web UI for the NosBazar bazaar search. It talks to the NosBazar
backend HTTP API on `localhost:8080`.

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

The app runs at `http://localhost:5173`. The backend must be running on
`localhost:8080` for the search and selectors to work — see the root
[README](../README.md#how-to-run-locally) or
[backend/README.md](../backend/README.md).

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