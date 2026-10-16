---
name: frontend-dev
description: Frontend dev commands for SvelteKit project
---

## Commands (from `frontend/`)

Follow the repository's `AGENTS.md` before using these commands.

| Command | Purpose |
|---------|---------|
| `npm run dev` | Dev server on `:5173` |
| `npm run build` | Production build |
| `npm run check` | TypeScript typecheck via svelte-check |
| `npm run lint` | ESLint |
| `npm run format` | Prettier formatting |

## Before committing
Run `npm run check && npm run lint`
