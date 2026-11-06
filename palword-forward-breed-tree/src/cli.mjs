import { readFile } from "node:fs/promises";
import { findBreedingPath } from "./solver.mjs";

const ROSTER_PATH = new URL("../data/pals.json", import.meta.url);

function parseArgs(argv) {
  const args = { maxDepth: 5 };
  for (let i = 0; i < argv.length; i++) {
    const a = argv[i];
    if (a === "--from" || a === "-f") args.from = argv[++i];
    else if (a === "--to" || a === "-t") args.to = argv[++i];
    else if (a === "--max-depth" || a === "-d") args.maxDepth = Number(argv[++i]);
    else if (!args.from) args.from = a;
    else if (!args.to) args.to = a;
  }
  return args;
}

function resolvePal(roster, name) {
  const norm = name.trim().toLowerCase();
  return roster.find(
    (p) => p.tribeName.toLowerCase() === norm || p.localizedName.toLowerCase() === norm
  );
}

function displayName(roster, tribeName) {
  return roster.find((p) => p.tribeName === tribeName)?.localizedName ?? tribeName;
}

async function main() {
  const argv = process.argv.slice(2);

  let roster;
  try {
    roster = JSON.parse(await readFile(ROSTER_PATH, "utf8"));
  } catch {
    console.error('No cached Pal data found. Run "npm run fetch" first.');
    process.exit(1);
  }

  if (argv.includes("--list") || argv.includes("-l")) {
    for (const p of [...roster].sort((a, b) => a.localizedName.localeCompare(b.localizedName))) {
      console.log(p.localizedName);
    }
    return;
  }

  const args = parseArgs(argv);
  if (!args.from || !args.to) {
    console.error(
      "Usage: node src/cli.mjs --from <Pal> --to <Pal> [--max-depth 5]\n" +
        '       node src/cli.mjs --list        (print every known Pal name)\n' +
        'Example: node src/cli.mjs --from Lamball --to "Blue Cat"'
    );
    process.exit(1);
  }

  const start = resolvePal(roster, args.from);
  const target = resolvePal(roster, args.to);
  if (!start) {
    console.error(`Unknown starting Pal: "${args.from}"`);
    process.exit(1);
  }
  if (!target) {
    console.error(
      `Unknown target Pal: "${args.to}" (note: this dataset predates Palworld 1.0, so ` +
        `1.0-era Pals like Loomen aren't in it yet)`
    );
    process.exit(1);
  }

  console.log(`Searching for a breeding path: ${start.localizedName} -> ${target.localizedName}`);
  console.log(`(roster: ${roster.length} Pals, max depth: ${args.maxDepth})\n`);

  const result = await findBreedingPath({
    startTribe: start.tribeName,
    targetTribe: target.tribeName,
    roster,
    maxDepth: args.maxDepth,
    onProgress: ({ depth, frontierSize }) =>
      process.stderr.write(`  searching depth ${depth} (${frontierSize} Pals to expand)...\n`),
  });

  if (!result.found) {
    console.log(
      `\nNo path found from ${start.localizedName} to ${target.localizedName} within ${args.maxDepth} breeding steps.`
    );
    console.log("Try increasing --max-depth, or confirm the target is reachable via breeding at all.");
    process.exit(2);
  }

  console.log(`\nFound a path in ${result.path.length - 1} breeding step(s):\n`);
  for (const step of result.path) {
    const name = displayName(roster, step.tribeName);
    if (!step.parentA) {
      console.log(`  [start] ${name}`);
    } else {
      const a = displayName(roster, step.parentA);
      const b = displayName(roster, step.parentB);
      console.log(`  ${a} + ${b}  -->  ${name}`);
    }
  }
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
