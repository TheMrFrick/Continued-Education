"use client";

export function setJSON<T>(key: string, value: T) {
  try {
    if (typeof window !== 'undefined') localStorage.setItem(key, JSON.stringify(value));
  } catch {
    // ignore storage errors
  }
}

export function getJSON<T>(key: string): T | null {
  try {
    if (typeof window !== 'undefined') {
      const v = localStorage.getItem(key);
      return v ? (JSON.parse(v) as T) : null;
    }
  } catch {
    // ignore parse/storage errors
  }
  return null;
}

export function uniqueCaseInsensitive(lines: string[]) {
  const seen = new Set<string>();
  const out: string[] = [];
  for (const l of lines) {
    const k = l.trim().toUpperCase();
    if (!k) continue;
    if (!seen.has(k)) {
      seen.add(k);
      out.push(k);
    }
  }
  return out;
}

export function downloadText(filename: string, content: string) {
  const blob = new Blob([content], { type: 'text/plain;charset=utf-8' });
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url;
  a.download = filename;
  document.body.appendChild(a);
  a.click();
  a.remove();
  URL.revokeObjectURL(url);
}
