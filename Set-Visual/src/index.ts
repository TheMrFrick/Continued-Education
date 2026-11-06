#!/usr/bin/env node
import { Command } from 'commander';
import fg from 'fast-glob';
import path from 'node:path';
import fs from 'node:fs/promises';
import sharp from 'sharp';

function escapeRegExp(str: string): string {
  return str.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

async function ensureDir(p: string): Promise<void> {
  await fs.mkdir(p, { recursive: true });
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
  const frontRegex = new RegExp(`\\b${escaped}-(\\d{2,3})\\b`, 'i');
  const backRegex = new RegExp(`\\b${escaped}-(\\d{2,3})_b\\b`, 'i');
  const ignoreRegex = /(\bback\b|\breverse\b|\brear\b|\bbackside\b)/i;

  const altRegex = new RegExp(`\\b${escaped}-(\\d{2,3})_(\\w+)\\b`, 'i');
  const frontMap = new Map<number, string>();
  const backMap = new Map<number, string>();
  const altMap = new Map<number, string[]>();
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
    // Alt art: SETKEY-NNN_SUFFIX (any suffix; _b already captured above)
    const ma = base.match(altRegex);
    if (ma) {
      const idx = parseInt(ma[1], 10);
      const list = altMap.get(idx) ?? [];
      list.push(abs);
      altMap.set(idx, list);
      continue;
    }
    const mf = base.match(frontRegex);
    if (mf) {
      const idx = parseInt(mf[1], 10);
      if (!frontMap.has(idx)) frontMap.set(idx, abs);
    }
  }

  // Sort each alt list by filename for deterministic ordering
  for (const [, list] of altMap) list.sort((a, b) => path.basename(a).localeCompare(path.basename(b)));

  const keys = new Set<number>([...frontMap.keys(), ...backMap.keys(), ...altMap.keys()]);
  const max = keys.size > 0 ? Math.max(...keys) : 0;
  return { frontMap, backMap, altMap, max };
}

async function listImagesInDirectory(dir: string): Promise<string[]> {
  const files = await fg(['*.{png,jpg,jpeg,webp}'], {
    cwd: dir,
    caseSensitiveMatch: false,
    onlyFiles: true,
    dot: false,
    unique: true,
    suppressErrors: true,
  });
  const sorted = files
    .map(rel => path.resolve(dir, rel))
    .sort((a, b) => path.basename(a).localeCompare(path.basename(b), undefined, { numeric: true, sensitivity: 'base' }));
  return sorted;
}

type BuildOptions = {
  columns: number;
  cardWidth: number;
  cardHeight: number;
  gutter: number;
  background: 'black' | 'transparent';
};

function gridPixelSize(cols: number, rows: number, opt: BuildOptions) {
  const width = cols * opt.cardWidth + (cols - 1) * opt.gutter;
  const height = rows * opt.cardHeight + (rows - 1) * opt.gutter;
  return { width, height };
}

async function buildGrid(
  orderedPaths: Array<string | undefined>,
  outPath: string,
  backPath: string,
  opt: BuildOptions
) {
  const cols = opt.columns;
  const rows = Math.ceil(orderedPaths.length / cols);
  const { width: canvasW, height: canvasH } = gridPixelSize(cols, rows, opt);

  const backBuf = await sharp(backPath)
    .resize(opt.cardWidth, opt.cardHeight, { fit: 'cover' })
    .toBuffer();

  const composites: sharp.OverlayOptions[] = [];

  for (let i = 0; i < orderedPaths.length; i++) {
    const r = Math.floor(i / cols);
    const c = i % cols;
    const left = c * (opt.cardWidth + opt.gutter);
    const top = r * (opt.cardHeight + opt.gutter);

    const srcPath = orderedPaths[i];
    if (!srcPath) continue;

    let inputBuf: Buffer;
    try {
      inputBuf = await sharp(srcPath)
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
      background: opt.background === 'transparent'
        ? { r: 0, g: 0, b: 0, alpha: 0 }
        : { r: 0, g: 0, b: 0, alpha: 1 },
    },
  })
    .composite(composites)
    .png({ quality: 90 })
    .toFile(outPath);
}

type InstagramOptions = {
  enabled: boolean;
  width: number;
  height: number;
  columns: number;
  rows: number;
};

async function buildFixedGrid(
  orderedPaths: Array<string | undefined>,
  outPath: string,
  backPath: string,
  opt: BuildOptions,
  fixedColumns: number,
  fixedRows: number
) {
  const cols = fixedColumns;
  const rows = fixedRows;
  const { width: canvasW, height: canvasH } = gridPixelSize(cols, rows, opt);

  const backBuf = await sharp(backPath)
    .resize(opt.cardWidth, opt.cardHeight, { fit: 'cover' })
    .toBuffer();

  const composites: sharp.OverlayOptions[] = [];
  const totalSlots = cols * rows;
  for (let i = 0; i < totalSlots; i++) {
    const r = Math.floor(i / cols);
    const c = i % cols;
    const left = c * (opt.cardWidth + opt.gutter);
    const top = r * (opt.cardHeight + opt.gutter);

    const srcPath = orderedPaths[i];
    if (!srcPath) continue;
    let inputBuf: Buffer;
    try {
      inputBuf = await sharp(srcPath)
        .resize(opt.cardWidth, opt.cardHeight, { fit: 'cover' })
        .toBuffer();
    } catch {
      inputBuf = backBuf;
    }

    composites.push({ input: inputBuf, left, top });
  }

  await sharp({
    create: {
      width: canvasW,
      height: canvasH,
      channels: 4,
      background: opt.background === 'transparent'
        ? { r: 0, g: 0, b: 0, alpha: 0 }
        : { r: 0, g: 0, b: 0, alpha: 1 },
    },
  })
    .composite(composites)
    .png({ quality: 90 })
    .toFile(outPath);
}

async function writeInstagramOutputsFromCards(
  orderedPaths: Array<string | undefined>,
  instaOutDir: string,
  setKey: string,
  buildOpt: BuildOptions,
  backPath: string,
  opt: InstagramOptions
): Promise<number> {
  if (!opt.enabled) return 0;

  await ensureDir(instaOutDir);

  const cols = Math.max(1, Math.floor(opt.columns));
  const rows = Math.max(1, Math.floor(opt.rows));
  const capacity = cols * rows;
  const slides = Math.max(1, Math.ceil(orderedPaths.length / capacity));

  for (let i = 0; i < slides; i++) {
    const start = i * capacity;
    const end = start + capacity;
    const pageCards = orderedPaths.slice(start, end);

    const outFile = path.join(instaOutDir, `${i + 1}_${setKey}.png`);
    const tmpFile = path.join(instaOutDir, `.__tmp_${i + 1}_${setKey}.png`);

    await buildFixedGrid(pageCards, tmpFile, backPath, buildOpt, cols, rows);

    await sharp(tmpFile)
      .resize(opt.width, opt.height, {
        fit: 'contain',
        background: buildOpt.background === 'transparent'
          ? { r: 0, g: 0, b: 0, alpha: 0 }
          : { r: 0, g: 0, b: 0, alpha: 1 },
      })
      .png({ quality: 90 })
      .toFile(outFile);

    await fs.unlink(tmpFile).catch(() => undefined);
  }

  return slides;
}

async function main() {
  const program = new Command();
  program
    .name('set-visual')
    .description('Compose a grid image for a DBS set (e.g., FB08) from local images, filling gaps with a card back.')
    .argument('[setKey]', 'Set key like FB08 or SB02 (omit when using --dir)')
    .option('-r, --root <dir>', 'Root folder containing images', 'E:/Deckplanet Data/Fusion World/Added To Deckplanet')
    .option('-b, --back <path>', 'Path to default back image (falls back to auto-detect within root)')
    .option('-o, --out <path>', 'Legacy: output image path (also writes new outputs to --out-dir)')
    .option('--out-dir <dir>', 'Output directory (defaults to ./out)', path.join(process.cwd(), 'out'))
    .option('--instagram', 'Also write Instagram-ready 4:5 crops to out/insta', false)
    .option('--insta-width <n>', 'Instagram output width in pixels', (v: string) => parseInt(v, 10), 1080)
    .option('--insta-height <n>', 'Instagram output height in pixels (4:5 is 1080x1350)', (v: string) => parseInt(v, 10), 1350)
    .option('--insta-cols <n>', 'Instagram page columns (default 5)', (v: string) => parseInt(v, 10), 5)
    .option('--insta-rows <n>', 'Instagram page rows (default 4)', (v: string) => parseInt(v, 10), 4)
    .option('--blank-background <mode>', 'Background for blank slots beyond discovered max (black|transparent)', 'black')
    .option('--max-fill <mode>', 'When --max is higher than discovered max: fill extra slots with back or leave blank (back|blank)', 'blank')
    .option('--dir <path>', 'Treat a directory as a pre-ordered set of images (no back-filling, sorted by filename)')
    .option('-c, --columns <n>', 'Grid columns', (v: string) => parseInt(v, 10), 8)
    .option('--card-width <n>', 'Card width in pixels', (v: string) => parseInt(v, 10), 300)
    .option('--card-height <n>', 'Card height in pixels', (v: string) => parseInt(v, 10), 420)
    .option('-g, --gutter <n>', 'Gutter size in pixels', (v: string) => parseInt(v, 10), 10)
    .option('--max <n>', 'Force maximum card index (e.g., 162)', (v: string) => parseInt(v, 10))
    .option('--alt-art <mode>', 'Alt art handling: additive (show alongside original) or overwrite (replace original). Omit to ignore alt arts.')
    .parse(process.argv);

  const setKey = (program.args[0] as string) ?? '';
  const opts = program.opts<{
    root: string;
    back?: string;
    out?: string;
    outDir: string;
    instagram: boolean;
    instaWidth: number;
    instaHeight: number;
    instaCols: number;
    instaRows: number;
    blankBackground: string;
    maxFill: string;
    dir?: string;
    columns: number;
    cardWidth: number;
    cardHeight: number;
    gutter: number;
    max?: number;
    altArt?: string;
  }>();
  if (!opts.dir && !setKey) {
    console.error('Missing <setKey>. Example: set-visual FB08');
    process.exit(1);
  }
  if (opts.dir && setKey) {
    console.error('Cannot specify both <setKey> and --dir');
    process.exit(1);
  }

  const imageRoot = path.resolve(opts.root);
  const baseOutDir = path.resolve(opts.outDir);
  const fullPicDir = path.join(baseOutDir, 'full_pic');
  const instaDir = path.join(baseOutDir, 'insta');
  await ensureDir(fullPicDir);
  if (opts.instagram) await ensureDir(instaDir);

  const setName = opts.dir ? path.basename(opts.dir) : setKey;
  const fullPicPath = path.join(fullPicDir, `1_${setName}.png`);

  const backPath = await resolveBackImage(opts.back, imageRoot);
  let ordered: string[];
  if (opts.dir) {
    ordered = await listImagesInDirectory(opts.dir);
  } else {
    const { frontMap, backMap, altMap, max } = await findSetImages(setKey, imageRoot);
    if (max === 0 && (opts.max ?? 0) === 0) {
      console.error(`No images found for set ${setKey} under ${imageRoot}`);
      process.exit(1);
    }
    const orderedWithUndefined: Array<string | undefined> = [];
    const maxIndex = Number.isFinite(opts.max) && (opts.max ?? 0) > 0 ? (opts.max as number) : max;
    const maxFill = String(opts.maxFill ?? '').toLowerCase() === 'back' ? 'back' : 'blank';
    const altMode = String(opts.altArt ?? '').toLowerCase() || undefined;
    for (let i = 1; i <= maxIndex; i++) {
      const beyondDiscoveredMax = i > max;
      if (beyondDiscoveredMax) {
        orderedWithUndefined.push(maxFill === 'back' ? backPath : undefined);
        continue;
      }

      const front = frontMap.get(i);
      const back = backMap.get(i);
      const alts = altMap.get(i) ?? [];

      if (altMode === 'additive') {
        if (front && back) {
          orderedWithUndefined.push(front, back, ...alts);
        } else if (front) {
          orderedWithUndefined.push(front, ...alts);
        } else if (back) {
          orderedWithUndefined.push(back, ...alts);
        } else if (alts.length > 0) {
          orderedWithUndefined.push(...alts);
        } else {
          orderedWithUndefined.push(backPath);
        }
      } else if (altMode === 'overwrite') {
        if (alts.length > 0) {
          orderedWithUndefined.push(...alts);
          if (back) orderedWithUndefined.push(back);
        } else if (front && back) {
          orderedWithUndefined.push(front, back);
        } else if (front) {
          orderedWithUndefined.push(front);
        } else if (back) {
          orderedWithUndefined.push(back);
        } else {
          orderedWithUndefined.push(backPath);
        }
      } else {
        if (front && back) {
          orderedWithUndefined.push(front, back);
        } else if (front) {
          orderedWithUndefined.push(front);
        } else if (back) {
          orderedWithUndefined.push(back);
        } else {
          orderedWithUndefined.push(backPath);
        }
      }
    }
    ordered = orderedWithUndefined.filter((p): p is string => p !== undefined);
  }

  console.log(`Composing ${ordered.length} images: ${opts.dir ? `directory ${opts.dir}` : `set ${setKey}`}`);
  const background = ((): 'black' | 'transparent' => {
    const v = String(opts.blankBackground ?? '').toLowerCase();
    return v === 'transparent' ? 'transparent' : 'black';
  })();
  await buildGrid(ordered, fullPicPath, backPath, {
    columns: opts.columns,
    cardWidth: opts.cardWidth,
    cardHeight: opts.cardHeight,
    gutter: opts.gutter,
    background,
  });

  // Statistics: real images vs backs/blanks
  const total = ordered.length;
  const realCount = ordered.filter(p => p !== backPath).length;
  const backOrBlankCount = total - realCount;
  const realPct = total > 0 ? ((realCount / total) * 100).toFixed(1) : '0.0';
  const backPct = total > 0 ? ((backOrBlankCount / total) * 100).toFixed(1) : '0.0';
  console.log(`Content: ${realCount} real images (${realPct}%) | ${backOrBlankCount} backs/blanks (${backPct}%)`);

  if (opts.out) {
    const legacyOutPath = path.resolve(opts.out);
    await ensureDir(path.dirname(legacyOutPath));
    await fs.copyFile(fullPicPath, legacyOutPath);
  }

  const instaCount = await writeInstagramOutputsFromCards(
    ordered,
    instaDir,
    setName,
    {
      columns: opts.columns,
      cardWidth: opts.cardWidth,
      cardHeight: opts.cardHeight,
      gutter: opts.gutter,
      background,
    },
    backPath,
    {
      enabled: opts.instagram,
      width: opts.instaWidth,
      height: opts.instaHeight,
      columns: opts.instaCols,
      rows: opts.instaRows,
    }
  );

  console.log(`Full image written to: ${fullPicPath}`);
  if (opts.instagram) console.log(`Instagram images written to: ${instaDir} (${instaCount} files)`);
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
