# Third-party mods and HD compatibility

This document records how Recreate handles the two installers found in the user's `Исталляторы.zip` archive. The installers themselves are not included in this repository.

## 1. HoMM3 HD Mod

**Archive file:** `HoMM3_HD_Latest_setup.exe`

This is the HD Mod for the original Heroes III executable. It is not a VCMI plugin and is not compatible with VCMI. Do not run its installer against a Recreate/VCMI installation and do not bundle it with Recreate.

For Recreate, use VCMI's own display and interface settings (resolution, scaling and other options provided by the selected VCMI build). Do not assume that VCMI has installed or activated the original HD Mod.

## 2. Horn of the Abyss installer

**Archive file:** `HotA_1.8.1_setup.exe`

This is the original HotA installer, intended for the original Heroes III installation. It is not a VCMI mod package and cannot simply be installed into Recreate's VCMI runtime.

VCMI has community-maintained HotA-related content/ports in its mod catalogue, but that is distinct from installing this original EXE. The HotA team has publicly expressed disapproval of third-party use/ports of HotA resources. Therefore:

- Recreate will not bundle the original HotA installer or HotA assets.
- Do not copy HotA files into the Recreate repository or release package without explicit permission from the rights holders.
- A community port must not be represented as an official HotA release or as approved by the HotA team.
- Any optional personal testing of a catalogue entry must be kept separate from the Recreate distribution and is not a dependency of the project.

## 3. Armageddon's Blade and Conflux

Do not remove or replace VCMI's native Armageddon's Blade support while evaluating third-party mods. Conflux remains a required playable faction in Recreate. Verify campaign availability, faction data, map-editor support and multiplayer behavior against the exact VCMI revision pinned by the project.

A mod must not silently disable the original expansion campaigns or Conflux.

## 4. Distribution rules

- Do not commit either installer from the archive.
- Do not redistribute original Heroes III or HotA game assets, maps, campaigns, music, videos or other proprietary resources.
- Keep optional community mods separate from the base Recreate package unless their licenses and permissions explicitly allow redistribution.
- Record the exact VCMI revision and mod version used for each compatibility test.
