# Heroes of Might and Magic III: Recreate

A Windows-focused project based on the VCMI engine. The planned public builds are separate **Win32 (32-bit)** and **x64 (64-bit)** packages. Linux is not a target for the initial release.

> Development status: early integration prototype. This repository is not a finished game or a release package.

## Current implementation

### Recreate Modding Studio — first native shell

The first C/Win32 application is a small native Windows front end for the real VCMI Map Editor. It can:

- start `VCMI_mapeditor.exe`;
- select and open `.h3m` / `.vmap` map files;
- select and open `.h3c` / `.vcmp` campaign files.

VCMI's map editor contains the campaign editor as an integrated component. Modding Studio passes the selected file as the editor's positional command-line argument; VCMI handles the actual editing. The shell does not pretend to implement map editing itself.

The editor executable must be supplied by a matching VCMI build and placed beside Modding Studio or in one of the searched subfolders (`VCMI/`, `VCMI/bin/`, `bin/`, or `external/vcmi/`). If it is not found, the user can locate it manually.

The Win32 and x64 front-end executables have been cross-compiled successfully. Runtime testing on Windows is still required.

## Required Heroes III content

Recreate must preserve and expose the original expansion content supported by the selected VCMI revision, not silently reduce the game to Restoration of Erathia.

### Armageddon's Blade

Required:
- restore the **Armageddon's Blade** expansion campaign and its campaign-selection entry;
- support the other original Armageddon's Blade campaign scenarios and their campaign progression;
- preserve campaign-specific victory/loss conditions, hero restrictions, carry-over rules, and scenario transitions;
- support the expansion's maps, heroes, artifacts, creatures, adventure objects, and gameplay rules.

### Conflux (Сопряжение)

Required:
- restore the **Conflux** town/faction as a playable faction;
- support its town screen, buildings, dwellings, creature line-up, heroes, portraits, recruitment, and faction-specific gameplay;
- ensure Conflux appears correctly in map setup, map editor, random maps, campaigns, and multiplayer where applicable.

These are project requirements, not claims that Recreate has implemented them already. During VCMI integration, verify each item against the exact upstream revision and test using game data supplied by the user. Do not redistribute original Heroes III assets or campaign files in this repository.

## Additional VCMI improvements

Recreate should extend the selected VCMI revision with the following goals. These are planned requirements, not claims that the features are already implemented:

### Display and resolution support

- Support common 4:3, 16:9, and 16:10 display aspect ratios, plus ultrawide displays where the renderer permits.
- Offer a range of practical resolutions, including high-resolution displays, while retaining a usable interface and readable text.
- Provide windowed and fullscreen modes, with reliable switching between them.
- Handle display scaling and high-DPI Windows settings without clipped menus, dialogs, or interface elements.
- Keep the original Heroes III visual proportions where appropriate; do not stretch the game image just to fill the screen.

### Expanded compatibility

- Maximize compatibility with lawful original Heroes III data installations and the supported expansion content.
- Improve compatibility with existing VCMI maps, campaigns, and mod packages, while clearly reporting unsupported or conflicting content.
- Preserve compatibility with the project's Win32 and x64 builds and test both architectures independently.
- Test across a broader range of supported Windows versions, display configurations, and hardware; document any known limitations.
- Track the exact VCMI revision and run regression tests when updating the engine so compatibility improvements do not break existing campaigns, maps, or multiplayer.

## Build on Windows

Requirements: Visual Studio 2022 with the Desktop development with C++ workload, CMake 3.20+, and Windows SDK.

```powershell
# Win32
cmake -S . -B build-win32 -A Win32
cmake --build build-win32 --config Release

# Win64
cmake -S . -B build-x64 -A x64
cmake --build build-x64 --config Release
```

## Cross-build on Linux (MinGW-w64)

With MinGW-w64 C toolchains installed:

```sh
cmake -S . -B build-win32 -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_C_COMPILER=i686-w64-mingw32-gcc-posix
cmake --build build-win32 --config Release

cmake -S . -B build-win64 -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc-posix
cmake --build build-win64 --config Release
```

## Architecture / next milestones

1. Build and package the VCMI runtime for Win32 and x64, using VCMI's own CMake project and dependency setup.
2. Verify upstream Armageddon's Blade campaign and Conflux support in the pinned VCMI revision; identify missing or regressed behavior.
3. Integrate the VCMI game client into the Recreate product identity, main menu, startup video, and settings.
4. Expand Modding Studio around VCMI's native map/campaign editor instead of reimplementing it.
5. Implement and test the Recreate lobby permissions, IP multiplayer workflow, and isolated voice-chat subsystem.
6. Add installers, language packs (`ru-RU`, `en-US`, `uk-UA`, `en-GB`), and release verification.

## Game data

Original Heroes III game data is not included. Users must provide a lawful installation/copy of the game data needed by VCMI. Do not commit proprietary game assets, campaign files, videos, or other original game resources to this repository.

## Third-party engine

VCMI is an independent open-source project. Recreate must preserve VCMI notices and comply with the license terms for any VCMI code used or modified. VCMI is not implied to endorse this project.

Relevant upstream source references:
- [VCMI Armageddon's Blade support configuration](https://github.com/vcmi/vcmi/blob/develop/config/gameConfig.json)
- [VCMI campaign format documentation](https://vcmi.eu/modders/Campaign_Format/)
- [VCMI Map Editor build target](https://github.com/vcmi/vcmi/blob/develop/mapeditor/CMakeLists.txt)
- [VCMI Map Editor startup and project-file argument handling](https://github.com/vcmi/vcmi/blob/develop/mapeditor/mainwindow.cpp)


## Intro credits

The opening video will include these acknowledgements:

1. **Vladislav Sanko (Vlandus3ya)** — Creator and Beta Tester of Heroes of Might and Magic III: Recreate.
2. **ChatGPT (GPT-6)** — Coding assistance with Recreate and Modding Studio.
3. **VCMI Team** — For the game engine and editor capabilities.

Credits appear in the intro video; there is no separate credits menu. Only the contributors listed above are included in the current credits plan.


## Third-party mod compatibility

The installers in the user's archive are **not** bundled with Recreate:

- The original **HoMM3 HD Mod** installer targets the original Heroes III executable and is not compatible with VCMI. Recreate should use VCMI's built-in display/interface settings instead.
- The original **Horn of the Abyss 1.8.1** installer is not a VCMI mod package and cannot simply be installed into VCMI. Community ports in the VCMI catalogue are separate projects; the HotA team has expressed disapproval of third-party use/ports of its resources. Recreate will not redistribute HotA installers or assets without explicit permission.

Armageddon's Blade and Conflux remain required project content and must not be removed when testing optional mods.

See [Third-party mod compatibility and distribution rules](docs/THIRD_PARTY_MODS.md) for the full policy.
