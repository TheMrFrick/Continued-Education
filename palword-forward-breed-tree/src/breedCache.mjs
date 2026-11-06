import { readFile, writeFile } from "node:fs/promises";
import { breed as breedRemote } from "./api.mjs";

const CACHE_PATH = new URL("../data/breed-cache.json", import.meta.url);

function cacheKey(a, b) {
  // Breeding in Palworld is order-independent.
  return [a, b].sort().join("|");
}

export class BreedCache {
  constructor(map) {
    this.map = map;
    this.dirty = false;
  }

  static async load() {
    try {
      const raw = await readFile(CACHE_PATH, "utf8");
      return new BreedCache(new Map(Object.entries(JSON.parse(raw))));
    } catch {
      return new BreedCache(new Map());
    }
  }

  async save() {
    if (!this.dirty) return;
    const obj = Object.fromEntries(this.map);
    await writeFile(CACHE_PATH, JSON.stringify(obj, null, 2));
    this.dirty = false;
  }

  async breed(a, b) {
    const key = cacheKey(a, b);
    if (this.map.has(key)) return this.map.get(key);
    const child = await breedRemote(a, b);
    this.map.set(key, child);
    this.dirty = true;
    return child;
  }
}
