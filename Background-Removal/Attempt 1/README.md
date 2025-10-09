# Background Removal – Attempt 1

A Next.js (TypeScript) app that removes image backgrounds in the browser using `@imgly/background-removal`. Supports drag & drop, preview, retry, and download.

## Tech
- Next.js 14
- React 18 + TypeScript
- `@imgly/background-removal` (runs fully in the browser)

## Run locally
```bash
# From the project root
cd "Background-Removal/Attempt 1"

# Install dependencies
npm install

# Start dev server
npm run dev
# Open http://localhost:3000
```

## Usage
1. Drag & drop or click to select an image (PNG/JPG/WEBP).
2. Wait for processing.
3. Preview the result.
4. Retry if needed.
5. Download as a transparent PNG.

## Notes
- Background removal runs client-side. The first run may take longer while the model loads.
- If model assets fail to load, ensure network access. Some versions may require hosting assets or configuring an asset path; consult the `@imgly/background-removal` docs.

## Structure
- `app/page.tsx` – Client UI (drag & drop, preview, retry, download)
- `app/layout.tsx` – App wrapper + global styles import
- `app/globals.css` – Minimal styling
- `next.config.mjs` – Next.js config
- `tsconfig.json` – TypeScript config
- `package.json` – Dependencies and scripts
