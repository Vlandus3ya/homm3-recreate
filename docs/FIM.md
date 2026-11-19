# Recreate File Integrity Manager (FIM)

FIM is a **read-only** audit component shared by the installer and Modding Studio.
It is not tied to the `AI` directory: it can inspect any selected game-data
folder or ZIP archive.

## Current implementation

Run:

```sh
python tools/recreate_fim.py "/path/to/Heroes3-data.zip" --json fim-report.json
python tools/recreate_fim.py "/path/to/Heroes3-data"
```

The first implementation:
- verifies ZIP CRC integrity;
- hashes every regular file with SHA-256;
- reports duplicate content and case-insensitive path collisions;
- flags unsafe archive paths rather than extracting them;
- inventories `.h3m` maps and applies Recreate curation labels;
- writes an optional JSON report suitable for later UI integration.

FIM does not extract, delete, modify, or overwrite source data. A duplicate hash is
a review signal, not an automatic deletion recommendation. Legitimate mods and
user-created files are allowed.

## Planned integration

1. Installer preflight: scan the chosen game-data source before import.
2. Modding Studio: scan selected folders and report missing, duplicate, or
   conflicting resources before packaging a mod.
3. Dependency checks: compare mod manifests with required VCMI/game components.
4. Safe import: stage to a temporary directory, verify again, then let the user
   confirm the destination changes.
5. Recovery: retain a manifest and rollback information for Recreate-managed files.

FIM reports file integrity and collisions. It does **not** prove that an official
Recreate binary is authentic. That is the responsibility of the separate RIAS
signature/provenance system, which is not implemented yet.
