# H3R-CL — Recreate Command Language

This document records the complete command set supplied by the project owner. Preserve spelling and casing exactly. No additional commands are defined here.

## Purchase and construction

```h3r
market="buy" 5-%crystals% for 15-%trees%
buyhero="gelu"
buyarmy=2-"Golden_dragons" for 4-%crystals% and 2400-%money%
buyincastle @Municipality@ for 5000-%money%
```

Meanings:
- `market="buy"`: market exchange/purchase.
- `buyhero`: recruit the named hero.
- `buyarmy`: recruit the specified number and creature type for the listed costs.
- `buyincastle`: buy/build the named castle structure for the listed cost. Example: Municipality.

## Attacks and spells

```h3r
attackhero=ignat(blue)
spell=lighting land on "titans"
attack="general_of_centaurs" hit "titans"
attack="shooter" shot "titans"
attackcastle=inferno(blue)
if we have skill "ballistics" shot "entry"
if we have skill "altillery" shot "magogs"
if we had skill "first aid" cure "millitary_dendroids"
```

Meanings:
- `attackhero`: attack the named enemy hero and player/color.
- `spell`: cast the named spell on the specified target.
- `attack ... hit`: attack the target with the named creature/group.
- `attack ... shot`: shoot the target with the named shooter/group.
- `attackcastle`: attack the named castle and player/color.
- `if we have skill`: conditional action when the hero has the named skill.
- `if we had skill`: conditional action using the exact supplied wording.
- `cure`: heal the named target.

## Capturing and artifacts

```h3r
capturemine "sulfur"
captureartifact "sign_of_courage"
Changeslotforartifact "dove_of_fearlessnes" to "sign_of_courage"
```

Meanings:
- `capturemine`: capture the named mine.
- `captureartifact`: pick up the named artifact.
- `Changeslotforartifact`: replace/swap the named artifact slot item to the specified artifact.

## Identifier spelling

These spellings are intentionally preserved exactly as supplied:
- `lighting`
- `altillery`
- `millitary_dendroids`
- `Changeslotforartifact`
- `dove_of_fearlessnes`

Do not auto-correct them. Confirmed command inventory: the commands listed in this document are the complete set currently provided by the project owner.

## Important implementation note

This is a language reference, not yet an executable parser. A runtime parser, validation rules, game-state adapter, and tests must be implemented before H3R-CL commands can control VCMI/Recreate in-game actions.
