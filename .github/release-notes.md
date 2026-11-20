## Install

1. **Mod** — put `archipelago.dusk` in your Dusklight mods folder:
   - Windows: `%APPDATA%\TwilitRealm\Dusklight\mods`
   - Linux: `~/.local/share/TwilitRealm/Dusklight/mods`
   - macOS: `~/Library/Application Support/TwilitRealm/Dusklight/mods`
2. **Apworld** — put `tp_dusklight.apworld` in your Archipelago `custom_worlds` folder.
3. **YAML** — grab a preset below, or generate a template from the Archipelago Launcher. Set `name:` to your slot name.

Then in Dusklight: use the arrows on the Play button to pick **Archipelago**, start a **new file**, and enter your server address, slot name and password. The seed builds from the server and you play.

Needs Dusklight 2.0.1 or newer. This mod carries its own copy of the randomizer, so Dusklight's built-in Randomizer doesn't need updating or removing; the two don't interfere.

## What's new in 1.9.0

Includes everything from the 1.8.0 beta.

- **Tracker strikes out checks nobody needs** (#3). When a player finishes their game, every check of yours holding one of their items is crossed out as "not needed" and left off the in-logic list. It updates live. Turn it off on the Status tab.
- **Covers tab** for game boxes. Every game in the room with the cover it got; covers the mod wasn't sure of are marked "best guess". Pick a game to search SteamGridDB and choose the right one, switch it to the Sol, or use your own art (the tab shows where to put it). Choices are remembered for every room. Names with a platform in them, like "Link's Awakening Switch", now find their game.
- **Gray boxes for filler** (#8, by @Hamsterface). An option on the Status page, off by default: boxes for items their world calls filler show in grays. Traps keep their colour.
- **Held items keep their own game box** (#4). Another Archipelago item spawning nearby while Link held one up could swap its cover, text and jingle for the other item's.
- **Hearts are no longer always progression** (#7). Heart Containers and Pieces of Heart only count as progression when the Hyrule Barrier or Big Key requirement is Hearts. The same goes for anything else only an unused setting needs (Magic Armor, Fused Shadows and Mirror Shards when nothing asks for them, Twilight Tears when the twilights are cleared). This is the apworld (1.9.1).
- **Tracker service for other mods** (#6). Other mods can read every check's state through `ApTrackerService`: see [`include/dusklight_archipelago/tracker.h`](https://github.com/noahsmaximum/dusklight-archipelago/blob/archipelago/include/dusklight_archipelago/tracker.h).

Multiworlds generated with apworld 1.9.0 keep working: just swap in the new `archipelago.dusk`. Use apworld 1.9.1 for new seeds.

## Updating

Update the mod and the apworld **together**. The generating host needs the new apworld for the new presets and options; if the logic data ever differs between a player's mod and the seed, the mod refuses it and says to update. Finish any multiworld already in progress on the versions you started it with.
