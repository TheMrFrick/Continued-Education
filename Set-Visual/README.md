# Set-Visual

Compose a grid image for a DBS Fusion World set (e.g., `FB08`, `SB02`) from local card images, filling gaps with a card back. Also supports directory mode to lay out images in filename order with no back-filling.

## Modes

### Set mode (default)
- Input images root default: `E:\Deckplanet Data\Fusion World\Added To Deckplanet`
- File name pattern detected: `<SETKEY>-<NN>` anywhere in the filename (e.g., `FB08-01.png`, `FB08-001.png`, `FB08-010 front.jpg`). Accepts 2 or 3 digits.
- Default placeholder back is `card_back.png` at the root of your image folder (auto-detected). You can override via `--back`.
- If a per-card back image exists with the suffix `_b` (e.g., `FB08-001_b.png`), and a front also exists, **both** will be included in order: front then back. If only one side exists, that side is used; otherwise the default `card_back.png` is used.

### Directory mode (`--dir`)
- Treats a folder as a pre-ordered set of images.
- Images are sorted by filename and placed into the grid in that order.
- No back-filling or set-key parsing; just layout the images as-is.

## Output
- Full image written to `out/full_pic/1_<SETKEY>.png` (or under `--out-dir`)
- Optional Instagram crops written to `out/insta/<N>_<SETKEY>.png` when `--instagram` is enabled

## Install

```bash
npm install
npm run build
```

If you want to run without building, you can use `npm run dev` (via tsx).

## Usage

```bash
# Basic (defaults: 8 columns, uses default root and `card_back.png` if present)
node dist/index.js FB08

# Directory mode: layout images in a folder by filename (no back-filling)
node dist/index.js --dir ./my-images

# Or with the installed CLI name after a global link or npx
npx tsx src/index.ts FB08

# Custom options
node dist/index.js FB08 \
  --root "E:/Deckplanet Data/Fusion World/Added To Deckplanet" \
  --back "E:/Deckplanet Data/Fusion World/Added To Deckplanet/card_back.png" \
  --out-dir "./out" \
  --instagram \
  --columns 8 \
  --card-width 300 \
  --card-height 420 \
  --gutter 10
```

## Options Reference

### Positional

| Argument | Description |
|---|---|
| `[setKey]` | Set key to build (e.g. `FB08`, `SB02`). Omit when using `--dir`. |

### Input

| Flag | Short | Default | Description |
|---|---|---|---|
| `--root <dir>` | `-r` | `E:/Deckplanet Data/Fusion World/Added To Deckplanet` | Root folder to scan for card images. |
| `--back <path>` | `-b` | auto-detect `card_back.png` in root | Path to the default back/placeholder image. Falls back to auto-discovery if not set. |
| `--dir <path>` | | | Directory mode: lay out all images in a folder by filename order. Cannot be combined with `[setKey]`. |
| `--max <n>` | | discovered max | Force the maximum card index (e.g. `162`). Slots beyond the discovered max are filled according to `--max-fill`. |
| `--alt-art <mode>` | | _(omit to ignore)_ | Alt art handling. See [Alt arts](#alt-arts) below. |

### Grid layout

| Flag | Short | Default | Description |
|---|---|---|---|
| `--columns <n>` | `-c` | `8` | Number of columns in the output grid. |
| `--card-width <n>` | | `300` | Width each card is resized to (pixels). |
| `--card-height <n>` | | `420` | Height each card is resized to (pixels). |
| `--gutter <n>` | `-g` | `10` | Gap between cards (pixels). |
| `--blank-background <mode>` | | `black` | Background color for empty slots beyond the discovered max. `black` or `transparent`. |
| `--max-fill <mode>` | | `blank` | What to place in slots beyond the discovered max when `--max` is set. `blank` or `back`. |

### Output

| Flag | Short | Default | Description |
|---|---|---|---|
| `--out-dir <dir>` | | `./out` | Base output directory. Full image is written to `<out-dir>/full_pic/1_<SETKEY>.png`. |
| `--out <path>` | `-o` | | Legacy single-file output path. When set, the full image is also copied to this path. |
| `--instagram` | | `false` | Write Instagram-ready resized pages to `<out-dir>/insta/<N>_<SETKEY>.png`. |
| `--insta-width <n>` | | `1080` | Instagram output width in pixels. |
| `--insta-height <n>` | | `1350` | Instagram output height in pixels (default is 4:5 ratio). |
| `--insta-cols <n>` | | `5` | Columns per Instagram page (cards per slide = `insta-cols × insta-rows`). |
| `--insta-rows <n>` | | `4` | Rows per Instagram page. |

Notes for Windows PowerShell or CMD:
- Paths with spaces must be quoted.
- You can use either backslashes `\` or forward slashes `/` in paths.

## Alt arts

Alt art files are detected by the `_SUFFIX` naming convention — any file matching `<SETKEY>-<NNN>_<SUFFIX>` (e.g. `FB10-013_PR`, `FB10-013_ALT`). The existing `_b` suffix for double-faced card backs is handled separately and is not treated as an alt art.

By default (no `--alt-art` flag) alt art files are ignored and the grid is built as normal.

### `--alt-art additive`

Each alt art image is placed **after** the regular card (and its DFC back if present) at that index. If a card has both a regular front and a `_PR` variant:

```
slot: FB10-013  →  FB10-013_PR  (FB10-013_b if it exists)
```

If multiple alt arts exist for one index they are placed in filename order.

### `--alt-art overwrite`

Alt art image(s) **replace** the regular front at that index. The DFC back (if any) is kept.

```
slot: FB10-013_PR  (FB10-013_b if it exists)
```

If no alt art exists for a given index the regular card is shown unchanged.

## How it works

1. Scans the `--root` folder recursively for images containing `<SETKEY>-<NNN>` (e.g., `FB08-001`, `FB08-123`).
2. Determines the maximum `NNN` present and builds a list from `001..max`.
3. For each missing index, uses the specified or auto-detected back image.
4. Uses `sharp` to resize and composite a grid, saving the final PNG to `out/full_pic/1_<SETKEY>.png` (or `--out-dir`).

## Example

To generate a grid for `SB02` with 12 columns and a 5px gutter:

```bash
node dist/index.js SB02 --columns 12 --gutter 5
```

## Troubleshooting

- If you see "Back image not found", pass `--back <path>` explicitly.
- Make sure your image files actually contain the `<SETKEY>-<NNN>` pattern in the filename.
- Very large sets or large card dimensions will produce large output images; adjust `--columns`, `--card-width`, `--card-height`, and `--gutter` if needed.
