# Source-map curation notes

This file records project curation decisions for the user-supplied Heroes III
archive. It is metadata only; no original map files are stored in this repository.

| Map | Recreate status | Notes |
|---|---|---|
| `World adventures.h3m` | In development | Preserve as a priority map; do not edit or overwrite the user's source copy. |
| `(CAMPAIGN) Moldovanka.h3m` | Excluded | Do not include in Recreate's curated map selection. The original archive remains untouched. |
| Other source maps | Pending review | Keep unchanged until compatibility and licensing review is complete. |

The supplied archive was successfully opened and its ZIP integrity test passed.
It contains 107 `.h3m` files. Header inspection found map-format versions 14 and
28, so the import pipeline must not assume a single map generation. The filename
`(CAMPAIGN) Moldovanka.h3m` is an `.h3m` map, not an `.h3c` campaign file.

Original maps and game assets are user-provided data and must not be committed to
this public repository. Recreate should import them locally and generate a
machine-readable manifest instead.
