# Set-Visual

Compose a grid image for a DBS Fusion World set (e.g., `FB08`, `SB02`) from local card images. Missing numbers between revealed cards are filled with a default back image, and when a specific per-card back exists it is preferred.

- Input images root default: `E:\Deckplanet Data\Fusion World\Added To Deckplanet`
- File name pattern detected: `<SETKEY>-<NNN>` anywhere in the filename (e.g., `FB08-001.png`, `FB08-010 front.jpg`).
- Default placeholder back is `card_back.png` at the root of your image folder (auto-detected). You can override via `--back`.
- If a per-card back image exists with the suffix `_b` (e.g., `FB08-001_b.png`), and a front also exists, **both** will be included in order: front then back. If only one side exists, that side is used; otherwise the default `card_back.png` is used.
- Output: a single PNG grid written to disk.

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

# Or with the installed CLI name after a global link or npx
npx tsx src/index.ts FB08

# Custom options
node dist/index.js FB08 \
  --root "E:/Deckplanet Data/Fusion World/Added To Deckplanet" \
  --back "E:/Deckplanet Data/Fusion World/Added To Deckplanet/card_back.png" \
  --out "./out/FB08-grid.png" \
  --columns 8 \
  --card-width 300 \
  --card-height 420 \
  --gutter 10
```

Notes for Windows PowerShell or CMD:
- Paths with spaces must be quoted.
- You can use either backslashes `\` or forward slashes `/` in paths.

## How it works

1. Scans the `--root` folder recursively for images containing `<SETKEY>-<NNN>` (e.g., `FB08-001`, `FB08-123`).
2. Determines the maximum `NNN` present and builds a list from `001..max`.
3. For each missing index, uses the specified or auto-detected back image.
4. Uses `sharp` to resize and composite a grid, saving the final PNG to `--out` (defaults to `./out/<SETKEY>-grid.png`).

## Example

To generate a grid for `SB02` with 12 columns and a 5px gutter:

```bash
node dist/index.js SB02 --columns 12 --gutter 5
```

## Troubleshooting

- If you see "Back image not found", pass `--back <path>` explicitly.
- Make sure your image files actually contain the `<SETKEY>-<NNN>` pattern in the filename.
- Very large sets or large card dimensions will produce large output images; adjust `--columns`, `--card-width`, `--card-height`, and `--gutter` if needed.
