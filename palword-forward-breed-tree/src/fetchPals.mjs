import { writeFile } from "node:fs/promises";
import { fileURLToPath } from "node:url";
import { fetchAllTribes } from "./api.mjs";

const OUT_PATH = new URL("../data/pals.json", import.meta.url);

function extractBaseForm(tribe) {
  // Each tribe groups variants (base, boss, gym boss, ...). Breeding always
  // operates on the base (non-boss) form, so that's the only one we keep.
  const base = tribe.pals.find((p) => !p.isBoss && !p.isGymBoss) ?? tribe.pals[0];
  return {
    tribeName: base.identity.tribeName,
    localizedName: base.identity.localizedName,
    element1: base.element1,
    element2: base.element2,
    breedingRank: base.breeding?.breedingRank ?? null,
  };
}

async function main() {
  console.log("Fetching all Pal tribes from palworldapi.azurewebsites.net ...");
  const tribes = await fetchAllTribes();
  const seen = new Map();
  for (const tribe of tribes) {
    const pal = extractBaseForm(tribe);
    seen.set(pal.tribeName, pal);
  }
  const pals = [...seen.values()]
    // A handful of tribes are unlocalized dev/placeholder entries
    // (e.g. "en_text" name, breedingRank 0) that aren't real obtainable Pals.
    .filter((p) => p.localizedName !== "en_text" && p.breedingRank > 0)
    .sort((a, b) => a.breedingRank - b.breedingRank);
  await writeFile(OUT_PATH, JSON.stringify(pals, null, 2));
  console.log(`Saved ${pals.length} pals to ${fileURLToPath(OUT_PATH)}`);
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
