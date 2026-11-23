# Heroes of Might and Magic III: Recreate

A Windows-focused project based on the VCMI engine. The planned public builds are separate **Win32 (32-bit)** and **x64 (64-bit)** packages. Linux is not a target for the initial release.

> **Development status: PAUSED.** Development and public releases are on hold while the project reviews licensing for original Heroes III materials and considers future team capacity. The repository and its development history are being preserved. This is a pause, not a cancellation.

## Project direction

Heroes III: Recreate aims to provide a near-original Heroes III experience in a standalone, convenient package. The project requires original game content for that goal, but no permission to redistribute Ubisoft-owned Heroes III materials has been confirmed.

Until the necessary permissions are confirmed:

- Do not include or publicly distribute proprietary Heroes III assets, music, sounds, maps, campaigns, or executable tools.
- Do not publish builds that bundle those materials.
- Keep existing source code, branches, and project history preserved.
- Any future release plan must be reviewed for third-party licensing and attribution requirements.

## Current implementation

### Recreate Modding Studio
The native Windows front end starts VCMI Map Editor and opens selected .h3m/.vmap maps or .h3c/.vcmp campaigns. It delegates editing to the real VCMI editor.

### No-original-files diagnostic mode
The Recreate launcher supports a diagnostic mode that can run without original Heroes III data:

- Launch `h3r.exe /check` (or `h3r.exe --check`) to inspect the installed VCMI runtime, client executable, game-data folder, map folder, and Mods folder.
- If the normal launcher detects missing game data or maps, it offers to run the diagnostic instead of trying to start a game that cannot load.
- This is an installation check, not a playable demo and not a replacement for the original game resources.

### Open Resources initiative
The repository contains an initial resource-pack scaffold and asset specification in [docs/OPEN_RESOURCES.md](docs/OPEN_RESOURCES.md). This material may support future work, but it does not replace the original resources required by the project's near-original direction.

## Windows installer pipeline

The repository contains a Windows Actions workflow that builds the VCMI runtime and the Recreate front ends for Win32 and x64, then compiles separate Inno Setup installers. These build files are preserved for reference; **the project is paused and no new public releases are planned during the pause**. The installer must not bundle proprietary Heroes III game data.

## Required Heroes III content

Recreate aims to preserve the original expansion content supported by the selected VCMI revision, including Armageddon's Blade and the Conflux faction. These are project requirements, not claims that every feature is implemented. Distribution of original Heroes III assets or campaign files requires appropriate permission.

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
