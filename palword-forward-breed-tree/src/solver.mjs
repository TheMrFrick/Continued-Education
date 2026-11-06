import { BreedCache } from "./breedCache.mjs";

async function mapWithConcurrency(items, limit, fn) {
  const results = new Array(items.length);
  let next = 0;
  async function worker() {
    while (next < items.length) {
      const i = next++;
      results[i] = await fn(items[i], i);
    }
  }
  await Promise.all(Array.from({ length: Math.min(limit, items.length) }, worker));
  return results;
}

/**
 * Breadth-first search over the breeding graph: starting from `startTribe`,
 * repeatedly breed every currently-reachable Pal against every obtainable
 * species until `targetTribe` is produced or `maxDepth` is hit. Palworld
 * breeding is order-independent and always yields an existing tribe, so the
 * search space is bounded by the roster size.
 */
export async function findBreedingPath({
  startTribe,
  targetTribe,
  roster,
  maxDepth = 5,
  concurrency = 12,
  onProgress,
}) {
  const cache = await BreedCache.load();
  const candidates = roster.map((p) => p.tribeName);

  const visited = new Map();
  visited.set(startTribe, { depth: 0, via: null });

  if (startTribe === targetTribe) {
    await cache.save();
    return { found: true, path: [{ tribeName: startTribe, via: null }] };
  }

  let frontier = [startTribe];
  let depth = 0;

  while (frontier.length > 0 && depth < maxDepth) {
    depth++;
    onProgress?.({ depth, frontierSize: frontier.length });

    const nextFrontier = [];
    let foundTarget = null;

    for (const parentA of frontier) {
      const results = await mapWithConcurrency(candidates, concurrency, async (parentB) => {
        try {
          const child = await cache.breed(parentA, parentB);
          return { parentA, parentB, child };
        } catch {
          return null;
        }
      });

      for (const r of results) {
        if (!r || visited.has(r.child)) continue;
        visited.set(r.child, { depth, via: [r.parentA, r.parentB] });
        nextFrontier.push(r.child);
        if (r.child === targetTribe) foundTarget = r.child;
      }

      await cache.save();
      if (foundTarget) break;
    }

    if (foundTarget) {
      return { found: true, path: reconstructPath(visited, targetTribe) };
    }
    frontier = nextFrontier;
  }

  await cache.save();
  return { found: false, path: null, visitedCount: visited.size };
}

// Each BFS step breeds the previous frontier Pal (`parentA`) against some
// wild/obtainable species (`parentB`) drawn from the full roster. `parentA`
// is therefore always the prior link in the chain, so the path back to the
// start is strictly linear.
function reconstructPath(visited, targetTribe) {
  const path = [];
  let cursor = targetTribe;
  while (cursor) {
    const node = visited.get(cursor);
    path.unshift({
      tribeName: cursor,
      depth: node.depth,
      parentA: node.via?.[0] ?? null,
      parentB: node.via?.[1] ?? null,
    });
    cursor = node.via ? node.via[0] : null;
  }
  return path;
}
