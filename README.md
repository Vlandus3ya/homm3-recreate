# Heroes of Might and Magic III: Recreate

A Windows-focused project based on the VCMI engine. The planned public builds are separate **Win32 (32-bit)** and **x64 (64-bit)** packages. Linux is not a target for the initial release.

> Development status: early integration prototype. This repository is not a finished game or a release package.

## Current implementation

### Recreate Modding Studio
The native Windows front end starts VCMI Map Editor and opens selected .h3m/.vmap maps or .h3c/.vcmp campaigns. It delegates editing to the real VCMI editor.

### No-original-files diagnostic mode
The Recreate launcher now supports a diagnostic mode that can run without original Heroes III data:

- Launch `h3r.exe /check` (or `h3r.exe --check`) to inspect the installed VCMI runtime, client executable, game-data folder, map folder, and Mods folder.
- If the normal launcher detects missing game data or maps, it offers to run the diagnostic instead of trying to start a game that cannot load.
- This is an installation check, not a playable demo and not a replacement for the original game resources.

### Open Resources initiative
Recreate will build its own original, openly licensed assets instead of copying proprietary Heroes III files. The first phase is a documented resource-pack scaffold and asset specification in [docs/OPEN_RESOURCES.md](docs/OPEN_RESOURCES.md). No third-party or original Heroes III graphics, music, sounds, maps, or campaigns are included.

## Windows installer pipeline
The Windows Actions workflow builds the VCMI runtime and the Recreate front ends for Win32 and x64, then compiles separate Inno Setup installers. The installer does not bundle proprietary Heroes III game data. Users may import their own lawful game data during setup.

## Required Heroes III content
Recreate aims to preserve the original expansion content supported by the selected VCMI revision, including Armageddon's Blade and the Conflux faction. These are project requirements, not claims that every feature is already implemented. Do not redistribute original Heroes III assets or campaign files.

## Build on Windows

Requirements: Visual Studio 2022 with the Desktop development with C++ workload, CMake 3.20+, and Windows SDK.

```powershell
cmake -S . -B build-win32 -A Win32
cmake --build build-win32 --config Release
cmake -S . -B build-x64 -A x64
cmake --build build-x64 --config Release
```

## Third-party engine

VCMI is an independent open-source project. Recreate must preserve VCMI notices and comply with the license terms for any VCMI code used or modified. VCMI is not implied to endorse this project.

## Intro credits

1. **Vladislav Sanko (Vlandus3ya)** — Creator and Beta Tester.
2. **ChatGPT (GPT-6)** — Coding assistance.
3. **VCMI Team** — Engine and editor capabilities.
