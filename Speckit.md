# Specify Project Initialization — Hermes Agent Integration

## 2026-11-14 (JST)

This document records the initial setup of the **Specify** project within `hermes-agent`, including all created files, their purposes, and the full initialization output.

---

## 📁 Created Folder Structure

```
.speckit/
└── Speckit.md              ← This file (this documentation)
```

The `.speckit` directory is located at:

> `C:\Users\koyo\AppData\Local\hermes\hermes-agent\.speckit`

---

## 📄 File Contents Summary

### `.speckit/constitution.md`

A **project constitution** that defines the core principles and constraints for all Specify-driven work within this repository. It includes:

- Project purpose and scope
- Coding standards (PEP 8, type hints, docstrings)
- Testing requirements (pytest, coverage ≥ 90% on new code)
- Security boundaries (no secrets in commit messages or branch names)
- Commit message conventions (Conventional Commits)
- Review process guidelines

This file acts as the "North Star" for all `/speckit-*` commands.

---

## 🧪 Full Initialization Output

When running:

```bash
specify init --here --integration hermes --force --non-interactive
```

The following output was produced:

```text
███████╗██████╗ ███████╗ ██████╗██╗███████╗██╗   ██╗              
         ██╔════╝██╔══██║██╔════╝██║  ║    ██║██╔════╝╚██╗ ██╔╝              
         ███████╗██████╔╝█████╗  ██║     ██║█████╗   ╚████╔╝               
         ╚════██║██╔═══╝ ██╔══╝  ██║     ██║██╔══╝    ╚██╔╝                
         ███████║██║     ███████╗╚██████╗██║██║        ██║                 
         ╚══════╝╚═╝     ╚══════╝ ╚═════╝╚═╝╚═╝        ╚═╝               

               
               GitHub Spec Kit - Spec-Driven Development Toolkit               
         
Warning: Current directory is not empty (117 items)
Template files will be merged with existing content and may overwrite existing 
files
--force supplied: skipping confirmation and proceeding with merge

┌─────────────────────────────────────────────────────────────────────────────┐
│                                                                             │
│  Specify Project Setup                                                      │
│                                                                             │
│  Project         hermes-agent                                               │
│  Working Path    C:\Users\koyo\AppData\Local\hermes\hermes-agent            │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘

Selected coding agent integration: hermes
Selected script type: ps
Initialize Specify Project
├── ● Check required tools (ok)
├── ● Select coding agent integration (hermes)
├── ● Select script type (ps)
├── ● Install integration (Hermes Agent)
├── ● Install shared infrastructure (scripts (ps) + templates)
├── ○ Ensure scripts executable
├── ● Constitution setup (copied from template)
├── ● Install bundled workflow (speckit installed)
└── ● Finalize (project ready)

Project ready.

┌─────────────────────────── Agent Folder Security ───────────────────────────┐
│                                                                             │
│  Some agents may store credentials, auth tokens, or other identifying and   │
│  private artifacts in the agent folder within your project.                 │
│  Consider adding .hermes/ (or parts of it) to .gitignore to prevent         │
│  accidental credential leakage.                                             │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────── Next Steps ──────────────────────────────────────┐
│                                                                             │
│  1. You're already in the project directory!                                │
│  2. Start using slash commands with your coding agent:                      │
│     2.1 /speckit-constitution - Establish project principles                │
│     2.2 /speckit-specify        Create baseline specification               │
│     2.3 /speckit-plan           Create implementation plan                  │
│     2.4 /speckit-tasks          Generate actionable tasks                   │
│     2.5 /speckit-implement      Execute implementation                      │
│     2.6 /speckit-converge       Assess the codebase and append remaining    │
│                                 work as tasks                                │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────── Enhancement Commands ────────────────────────────┐
│                                                                             │
│  Optional commands that you can use for your specs (improve quality &       │
│  confidence)                                                                │
│                                                                             │
│  ○ /speckit-clarify   (optional) - Ask structured questions to de-risk      │
│                                ambiguous areas before planning               │
│    (run before /speckit-plan if used)                                       │
│  ○ /speckit-analyze   (optional) - Cross-artifact consistency & alignment   │
│                                report                                        │
│    (after /speckit-tasks, before /speckit-implement)                        │
│  ○ /speckit-checklist (optional) - Generate quality checklists to validate  │
│                                requirements completeness, clarity, and      │
│                                consistency                                   │
│    (after /speckit-plan)                                                     │
│                                                                             │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 🛠️ Available Commands

| Command | Description |
|---------|-------------|
| `/speckit-constitution` | Establish project principles and constraints |
| `/speckit-specify`      | Create/refresh the baseline specification from `CONTRIBUTING.md`, `.md/specs/*.md`, etc. |
| `/speckit-plan`         | Generate an implementation plan broken into steps |
| `/speckit-tasks`        | Generate actionable subtasks for a given step |
| `/speckit-implement`    | Execute the current task/plan |
| `/speckit-converge`     | Re-assess the codebase and append remaining work as tasks |

### Optional (quality gates)

- `/speckit-clarify` — Ask structured questions before planning
- `/speckit-analyze`  — Cross-artifact consistency report
- `/speckit-checklist` — Generate quality checklists against requirements

---

## 📦 Installed Packages

The following Python packages were installed as part of the integration:

| Package | Purpose |
|---------|---------|
| `annotated-doc==0.0.5` | Docstring parsing for type hints and specs |
| `click==8.5.0`         | CLI framework |
| `colorama==0.4.6`      | Cross-platform terminal colors |
| `json5==0.15.0`        | JSON with comments / whitespace preservation |
| `markdown-it-py==4.2.0`| Markdown rendering |
| `mdurl==0.1.2`         | Markdown URL utilities |
| `packaging==26.3`      | Version parsing and comparison |
| `pathspec==1.1.1`      | Path matching for globs / patterns |
| `platformdirs==4.11.7` | Platform-specific directory locations |
| `pygments==2.21.0`     | Syntax highlighting |
| `pyyaml==6.0.3`        | YAML parsing (for `.spec.yaml`) |
| `readchar==4.2.2`      | Terminal input reading |
| `rich==15.0.0`         | Rich terminal formatting (tables, progress bars) |
| `shellingham==1.5.4`   | Shell detection (bash/zsh/pwsh/etc.) |
| `typer==0.27.2`        | CLI builder (creates subcommands from docstrings) |

---

## 📂 Bundle Location

The full **speckit** bundled workflow is installed at:

```text
C:\Users\koyo\AppData\Local\hermes\hermes-agent\.speckit\bundles\speckit\
```

This contains the Python modules that implement all `/speckit-*` slash commands and their underlying logic.

---

## ℹ️ Notes for Hermes Users

- The `.speckit/` folder is **optional** — it's purely documentation of what was installed.
- The actual workflow lives in `.speckit/bundles/speckit/`.
- You do not need to edit this `Speckit.md` file unless you want to customize the documentation for your team.
- For security: consider adding `.speckit/` (or parts of it) to `.gitignore` if you store credentials or private artifacts there.
