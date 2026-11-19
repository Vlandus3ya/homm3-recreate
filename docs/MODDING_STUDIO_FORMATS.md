# Recreate Modding Studio: proposed native formats

These are Recreate formats, not original Heroes III formats. The first version
will use UTF-8 JSON so files are inspectable, diffable, and easy to validate.
Each document has a required `format`, `schemaVersion`, and stable `id`.
Binary art/audio assets are referenced by relative paths and SHA-256 hashes;
they are not embedded as proprietary game assets in the repository.

## Common envelope

```json
{
  "format": "h3a",
  "schemaVersion": 1,
  "id": "example.unique-id",
  "name": "Example",
  "assets": [],
  "data": {}
}
```

Unknown fields must be preserved by editors where possible. Unknown major schema
versions must be opened read-only rather than silently rewritten. All referenced
paths must remain inside the project directory.

## Editor formats

| Editor | Extension | Format identifier | Intended content |
|---|---|---|---|
| Artifact Editor | `.h3a` | `h3a` | Artifact metadata, slots, bonuses, icons, descriptions |
| Model Editor | `.h3md` | `h3md` | Creature/object model metadata, animation and asset references |
| Spell Editor | `.h3mc` | `h3mc` | Spell school, cost, targeting, effects, visuals and sound references |
| Skill Editor | `.h3ss` | `h3ss` | Hero skills: name, description, levels, effects, parameters and progression |

The **Skill Editor** edits hero skills (for example, Archery, Tactics, Leadership,
Luck, and Necromancy). It is not an editor for creature abilities; those are a
separate possible category.

## VCMI interoperability

Native Recreate documents are authoring sources. Exporters must translate supported
fields into the exact JSON/mod structure accepted by the pinned VCMI revision.
Do not assume a Recreate extension is directly loadable by VCMI. Unsupported
properties must be reported in an export validation report, not silently dropped.

## Security and validation

- Reject absolute paths and `..` traversal in asset references.
- Validate types, ranges, required fields, and schema versions.
- Do not execute scripts embedded in a project file during preview.
- Treat external assets as untrusted; scan and hash them before packaging.
- Keep official engine binaries and proprietary Heroes III data out of mod packages.
