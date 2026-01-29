import { promises as fs } from "fs";
import * as path from "path";

/**
 * Prefix lists used to decide where files should be moved based on filename.startsWith().
 *
 * Populate these arrays with the prefixes that identify each destination.
 * Example:
 *  mastersPrefixes = ["BT-01", "ST-"].
 *  fusionWorldPrefixes = ["FW-", "FWP-"]
 */
export const mastersPrefixes: string[] = ["BT", "SD", "P-", "TB", "XD", "EX", "EB", "DB"];
export const fusionWorldPrefixes: string[] = ["FB", "FS", "FP", "SB"];

/**
 * Default directories used by this script.
 * - SOURCE_DIR: Where to scan for files.
 * - DEST_MASTERS_DIR: Destination root for "masters" files.
 * - DEST_FUSION_WORLD_DIR: Destination root for "fusion-world" files.
 *
 * Update these if your destinations differ, or override via function options.
 */
export const SOURCE_DIR = "E:\\Deckplanet Data\\deckplanet_card_images";
export const DEST_MASTERS_DIR = "E:\\Deckplanet Data\\deckplanet_card_images\\dbs_masters";
export const DEST_FUSION_WORLD_DIR = "E:\\Deckplanet Data\\deckplanet_card_images\\fusion_world";

type Destination = "masters" | "fusion-world" | null;

/**
 * Determine destination bucket by checking if the filename starts with any of the provided prefixes.
 *
 * @param fileName The base file name (no directory components)
 * @param opts.prefixesMasters Prefixes for the "masters" bucket
 * @param opts.prefixesFusionWorld Prefixes for the "fusion-world" bucket
 * @param opts.caseInsensitive If true, comparison is case-insensitive (default: false)
 */
function decideDestination(
  fileName: string,
  opts: {
    prefixesMasters: string[];
    prefixesFusionWorld: string[];
    caseInsensitive?: boolean;
  }
): Destination {
  const { prefixesMasters, prefixesFusionWorld, caseInsensitive = true } = opts;

  const normalize = (s: string) => (caseInsensitive ? s.toLowerCase() : s);
  const name = normalize(fileName);

  const mastersHit = prefixesMasters.some((p) => name.startsWith(normalize(p)));
  if (mastersHit) return "masters";

  const fusionHit = prefixesFusionWorld.some((p) => name.startsWith(normalize(p)));
  if (fusionHit) return "fusion-world";

  return null;
}

/**
 * Scan a directory and move files to destination folders based on filename.startsWith() rules.
 *
 * This is scaffolded to be safe by default with dryRun=true. Set dryRun=false to actually rename (move) files.
 *
 * @param options Optional configuration
 * @param options.sourceDir Source directory to scan. Defaults to `SOURCE_DIR`.
 * @param options.mastersDir Destination directory for "masters". Defaults to `DEST_MASTERS_DIR`.
 * @param options.fusionWorldDir Destination directory for "fusion-world". Defaults to `DEST_FUSION_WORLD_DIR`.
 * @param options.caseInsensitive If true, prefix matching is case-insensitive. Default false.
 * @param options.dryRun If true, only logs planned actions without moving files. Default true.
 *
 * Notes:
 * - Files that match neither prefix list are logged with `[NO MATCH]` so you can update the prefix arrays.
 */
export async function moveFilesByPrefix(options: {
  sourceDir?: string;
  mastersDir?: string;
  fusionWorldDir?: string;
  caseInsensitive?: boolean;
  dryRun?: boolean;
} = {}): Promise<void> {
  const {
    sourceDir = SOURCE_DIR,
    mastersDir = DEST_MASTERS_DIR,
    fusionWorldDir = DEST_FUSION_WORLD_DIR,
    caseInsensitive = true,
    dryRun = true,
  } = options;

  // Ensure destination directories exist (if not dry-run)
  if (!dryRun) {
    await fs.mkdir(mastersDir, { recursive: true }).catch(() => void 0);
    await fs.mkdir(fusionWorldDir, { recursive: true }).catch(() => void 0);
  }

  const entries = await fs.readdir(sourceDir, { withFileTypes: true });
  for (const entry of entries) {
    if (!entry.isFile()) continue;

    const srcPath = path.join(sourceDir, entry.name);
    const destType = decideDestination(entry.name, {
      prefixesMasters: mastersPrefixes,
      prefixesFusionWorld: fusionWorldPrefixes,
      caseInsensitive,
    });

    if (!destType) {
      console.warn(`[NO MATCH] ${entry.name} — update mastersPrefixes/fusionWorldPrefixes if needed`);
      continue;
    }

    const destRoot = destType === "masters" ? mastersDir : fusionWorldDir;
    const destPath = path.join(destRoot, entry.name);

    if (dryRun) {
      console.log(`[DRY RUN] Would move: ${srcPath} -> ${destPath}`);
    } else {
      await fs.rename(srcPath, destPath);
      console.log(`Moved: ${srcPath} -> ${destPath}`);
    }
  }
}

// Example usage (uncomment and adjust prefixes, then run with ts-node):
// mastersPrefixes.push("BT-", "ST-");
// fusionWorldPrefixes.push("FW-", "FWP-");
moveFilesByPrefix({dryRun: false}).catch(console.error);

