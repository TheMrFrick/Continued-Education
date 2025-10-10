#!/usr/bin/env node
import { Command } from 'commander';
import fg from 'fast-glob';
import path from 'node:path';
import fs from 'node:fs/promises';
import sharp from 'sharp';

function escapeRegExp(str: string): string {
  return str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

async function fileExists(p: string): Promise<boolean> {
  try {
    await fs.access(p);
    return true;
  } catch {
    return false;
  }
}

async function resolveBackImage(backOpt: string | undefined, root: string): Promise<string> {
  // 1) Explicit path wins
  if (backOpt && (await fileExists(backOpt))) return backOpt;
  // 2) Prefer a canonical default name in root: card_back.png
  const canonical = path.resolve(root, 'card_back.png');
  if (await fileExists(canonical)) return canonical;
  // 3) Try to auto-discover
  const candidates = await fg([
    '**/card_back.{png,jpg,jpeg,webp}',
    '**/*card*back*.{png,jpg,jpeg,webp}',
    '**/*back*card*.{png,jpg,jpeg,webp}',
    '**/back.{png,jpg,jpeg,webp}',
  ], { cwd: root, caseSensitiveMatch: false, onlyFiles: true, dot: false });

  if (candidates.length > 0) return path.resolve(root, candidates[0]);

  throw new Error('Back image not found. Expected card_back.png in root or pass --back <path>.');
}

async function findSetImages(setKey: string, root: string) {
  const patterns = [`**/*${setKey}-*.{png,jpg,jpeg,webp}`];
  const files = await fg(patterns, {
    cwd: root,
    caseSensitiveMatch: false,
    onlyFiles: true,
    dot: false,
    unique: true,
    suppressErrors: true,
  });

  const escaped = escapeRegExp(setKey);
  const frontRegex = new RegExp(`\\b${escaped}-(\\d{3})\\b`, 'i');
  const backRegex = new RegExp(`\\b${escaped}-(\\d{3})_b\\b`, 'i');
  const ignoreRegex = /(\bback\b|\breverse\b|\brear\b|\bbackside\b)/i;

  const frontMap = new Map<number, string>();
  const backMap = new Map<number, string>();
  for (const rel of files) {
    const abs = path.resolve(root, rel);
    const base = path.basename(abs);
    if (ignoreRegex.test(base)) continue;
    const mb = base.match(backRegex);
    if (mb) {
      const idx = parseInt(mb[1], 10);
      if (!backMap.has(idx)) backMap.set(idx, abs);
      continue;
    }
    const mf = base.match(frontRegex);
    if (mf) {
      const idx = parseInt(mf[1], 10);
      if (!frontMap.has(idx)) frontMap.set(idx, abs);
    }
  }

  const keys = new Set<number>([...frontMap.keys(), ...backMap.keys()]);
  const max = keys.size > 0 ? Math.max(...keys) : 0;
  return { frontMap, backMap, max };
}

type BuildOptions = {
  columns: number;
  cardWidth: number;
  cardHeight: number;
  gutter: number;
};

async function buildGrid(
  orderedPaths: string[],
  outPath: string,
  backPath: string,
  opt: BuildOptions
) {
  const cols = opt.columns;
  const rows = Math.ceil(orderedPaths.length / cols);
  const canvasW = cols * opt.cardWidth + (cols - 1) * opt.gutter;
  const canvasH = rows * opt.cardHeight + (rows - 1) * opt.gutter;

  const backBuf = await sharp(backPath)
    .resize(opt.cardWidth, opt.cardHeight, { fit: 'cover' })
    .toBuffer();

  const composites: sharp.OverlayOptions[] = [];

  for (let i = 0; i < orderedPaths.length; i++) {
    const r = Math.floor(i / cols);
    const c = i % cols;
    const left = c * (opt.cardWidth + opt.gutter);
    const top = r * (opt.cardHeight + opt.gutter);

    let inputBuf: Buffer;
    try {
      inputBuf = await sharp(orderedPaths[i])
        .resize(opt.cardWidth, opt.cardHeight, { fit: 'cover' })
        .toBuffer();
    } catch {
      inputBuf = backBuf; // fallback if a specific front image fails to load
    }

    composites.push({ input: inputBuf, left, top });
  }

  await sharp({
    create: {
      width: canvasW,
      height: canvasH,
      channels: 4,
      background: { r: 0, g: 0, b: 0, alpha: 1 },
    },
  })
    .composite(composites)
    .png({ quality: 90 })
    .toFile(outPath);
}

async function main() {
  const program = new Command();
  program
    .name('set-visual')
    .description('Compose a grid image for a DBS set (e.g., FB08) from local images, filling gaps with a card back.')
    .argument('<setKey>', 'Set key like FB08 or SB02')
    .option('-r, --root <dir>', 'Root folder containing images', 'E:/Deckplanet Data/Fusion World/Added To Deckplanet')
    .option('-b, --back <path>', 'Path to default back image (falls back to auto-detect within root)')
    .option('-o, --out <path>', 'Output image path (defaults to ./out/<setKey>-grid.png)')
    .option('-c, --columns <n>', 'Grid columns', (v: string) => parseInt(v, 10), 8)
    .option('--card-width <n>', 'Card width in pixels', (v: string) => parseInt(v, 10), 300)
    .option('--card-height <n>', 'Card height in pixels', (v: string) => parseInt(v, 10), 420)
    .option('-g, --gutter <n>', 'Gutter size in pixels', (v: string) => parseInt(v, 10), 10)
    .option('--max <n>', 'Force maximum card index (e.g., 162)', (v: string) => parseInt(v, 10))
    .parse(process.argv);

  const setKey = (program.args[0] as string) ?? '';
  if (!setKey) {
    console.error('Missing <setKey>. Example: set-visual FB08');
    process.exit(1);
  }
  const opts = program.opts<{
    root: string;
    back?: string;
    out?: string;
    columns: number;
    cardWidth: number;
    cardHeight: number;
    gutter: number;
    max?: number;
  }>();

  const imageRoot = path.resolve(opts.root);
  const outPath = path.resolve(
    opts.out ?? path.join(process.cwd(), 'out', `${setKey}-grid.png`)
  );

  await fs.mkdir(path.dirname(outPath), { recursive: true });

  const backPath = await resolveBackImage(opts.back, imageRoot);
  const { frontMap, backMap, max } = await findSetImages(setKey, imageRoot);

  if (max === 0 && (opts.max ?? 0) === 0) {
    console.error(`No images found for set ${setKey} under ${imageRoot}`);
    process.exit(1);
  }

  const ordered: string[] = [];
  const maxIndex = Number.isFinite(opts.max) && (opts.max ?? 0) > 0 ? (opts.max as number) : max;
  for (let i = 1; i <= maxIndex; i++) {
    const front = frontMap.get(i);
    const back = backMap.get(i);
    if (front && back) {
      // When both exist, include both: front then back
      ordered.push(front, back);
    } else if (front) {
      ordered.push(front);
    } else if (back) {
      ordered.push(back);
    } else {
      ordered.push(backPath);
    }
  }

  console.log(`Composing ${ordered.length} cards: set ${setKey}, max index ${maxIndex}`);
  await buildGrid(ordered, outPath, backPath, {
    columns: opts.columns,
    cardWidth: opts.cardWidth,
    cardHeight: opts.cardHeight,
    gutter: opts.gutter,
  });

  console.log(`Output written to: ${outPath}`);
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
