# palword-forward-breed-tree

Given a starting Pal and a target Pal, finds the shortest chain of breeding
steps to get from one to the other.

## How it works

Palworld breeding is order-independent and deterministic: breeding any two
Pals always produces one specific existing Pal. This tool treats
[palworldapi.azurewebsites.net](https://github.com/PalworldDataTools/PalworldApi)
as the source of truth for breeding results (it implements the game's real
formula, including special-case overrides) and does a breadth-first search:
starting from your chosen Pal, it breeds it against every obtainable species,
looks at all the resulting children, and repeats until the target shows up or
`--max-depth` is reached. Every breeding call is cached to disk
(`data/breed-cache.json`) so repeat/related searches are near-instant.

At each step, `parentA` is always the previous link in the chain and
`parentB` is assumed to be freely catchable in the wild — the tool doesn't
currently account for parents that themselves require a separate breeding
chain to obtain.

## Caveat: pre-1.0 dataset

The underlying API's data predates the Palworld 1.0 launch — it covers 137
Pals and does **not** include 1.0-era additions like Loomen. Targeting a
1.0-only Pal will report it as unknown. No free, hosted, 1.0-updated Palworld
API was found as of this writing.

## Usage

```bash
npm run fetch                 # one-time: cache the full Pal roster to data/pals.json
node src/cli.mjs --list       # see every known Pal name
node src/cli.mjs --from Lamball --to Blazehowl --max-depth 5
```

Output:

```
Found a path in 2 breeding step(s):

  [start] Lamball
  Lamball + Blazamut  -->  Rayhound
  Rayhound + Univolt  -->  Blazehowl
```

If no path is found within `--max-depth` (default 5), either raise the depth
or the target may not be reachable via normal breeding at all (some
raid-boss/mount Pals can't be bred, only captured).
