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
2. Integrate the VCMI game client into the Recreate product identity, main menu, startup video, and settings.
3. Expand Modding Studio around VCMI's native map/campaign editor instead of reimplementing it.
4. Implement and test the Recreate lobby permissions, IP multiplayer workflow, and isolated voice-chat subsystem.
5. Add installers, language packs (`ru-RU`, `en-US`, `uk-UA`, `en-GB`), and release verification.

## Game data

Original Heroes III game data is not included. Users must provide a lawful installation/copy of the game data needed by VCMI. Do not commit proprietary game assets to this repository.

## Third-party engine

VCMI is an independent open-source project. Recreate must preserve VCMI notices and comply with the license terms for any VCMI code used or modified. VCMI is not implied to endorse this project.

Relevant upstream source references:
- [VCMI Map Editor build target](https://github.com/vcmi/vcmi/blob/develop/mapeditor/CMakeLists.txt)
- [VCMI Map Editor startup and project-file argument handling](https://github.com/vcmi/vcmi/blob/develop/mapeditor/mainwindow.cpp)
