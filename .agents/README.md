# Shared agent skills

Repository-wide instructions live in `AGENTS.md`. Reusable workflows live in
`.agents/skills/<skill-name>/SKILL.md`, using standard `name` and `description`
frontmatter and plain Markdown instructions.

| Agent | Project discovery |
|-------|-------------------|
| Codex | Reads `AGENTS.md` and discovers `.agents/skills` |
| OpenCode | Reads `AGENTS.md` and discovers `.agents/skills` |

Edit workflows only in `.agents/skills`. No tool-specific adapters or settings
are maintained. Agents that do not discover this directory automatically must
be directed to read the relevant `SKILL.md` explicitly; Claude Code does not
automatically discover `.agents/skills`.

The ignored `.opencode` package files and `node_modules` are tool-local runtime
files, not shared skills. Leave them in `.opencode`; do not move or commit them.

After migration, restart the agent or reload its skills. Check skill discovery
in each agent before relying on automatic invocation. See the official
[Codex](https://developers.openai.com/codex/skills),
[OpenCode](https://opencode.ai/v2/docs/skills), and
[Claude Code](https://code.claude.com/docs/en/skills) documentation.
