# Recreate Bots

This folder contains the ten named bot profiles and the H3R-CL command-language reference.

## Bot IDs
- `enemyred`
- `enemyblue`
- `enemybrown`
- `enemygreen`
- `enemyorange`
- `enemypurple`
- `enemycyan`
- `enemypink`
- `enemywhite`
- `enemyblack`

Each bot uses the same architecture:
- one controller (управляющий);
- one executor (ведомый);
- two command brains: ChatGPT (primary) and Gemini (backup).

The controller sends the current game situation to the command brain. The command brain decides what the bot should do, buy, pick up, capture, build, or attack. The controller translates that decision into orders for the executor.

The executor handles in-game hero movement, capturing, pickups, recruitment, market purchases, artifacts, spells, and castle actions.

## Connection policy
- Look for the installed ChatGPT or Gemini app first.
- If unavailable, use a browser to open the official service URL.
- Connection timeout: 5 minutes.
- ChatGPT is primary; Gemini is the fallback.
- On Save & Exit, close the browser instance/session started for the bot, independent of browser vendor. Do not close unrelated browser sessions.

## H3R-CL
See [H3R-CL.md](H3R-CL.md) for the complete currently supplied command inventory.

## Implementation status
These files define the agreed profiles and specification. They do not yet make the bots operational: runtime integration still needs a safe AI connection mechanism, a H3R-CL parser, a VCMI game-state/action adapter, and automated tests. No API keys or user credentials are stored here.
