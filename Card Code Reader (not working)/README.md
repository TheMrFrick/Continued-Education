# Card Code Reader

Camera OCR app to capture alphanumeric codes from physical cards using Next.js 14, TypeScript, and Tesseract.js.

## A) Setup

```bash
# From your workspace root
mkdir -p "Card Code Reader"
cd "Card Code Reader"

# Install dependencies
npm install

# (optional) Disable Next telemetry
npx next telemetry disable
```

## B) File tree (App Router)

```
Card Code Reader/
├─ app/
│  ├─ api/
│  │  └─ save/
│  │     └─ route.ts
│  ├─ layout.tsx
│  └─ page.tsx
├─ components/
│  ├─ CameraFeed.tsx
│  ├─ DetectedTextArea.tsx
│  └─ OcrControls.tsx
├─ data/
│  └─ .gitkeep
├─ lib/
│  ├─ ocr.ts
│  └─ storage.ts
├─ styles/
│  └─ globals.css
├─ next-env.d.ts
├─ next.config.mjs
├─ package.json
├─ postcss.config.js
├─ tailwind.config.ts
├─ tsconfig.json
└─ README.md
```

## C) Features

- Camera device selection with `enumerateDevices()` and persisted choice.
- Live preview using `getUserMedia`, ROI overlay adjustable with sliders.
- OCR pipeline using Tesseract.js (eng), grayscale + threshold pre-processing.
- Regex extraction and normalization of groups (4 or 5 chars per group).
- Auto-append with min confidence 70, 2s lockout, manual Capture bypass.
- Results textarea with dedupe, save to server file, download `.txt`.
- Persistence via localStorage and server POST `/api/save`.
- Accessible buttons, Tailwind styling, semi-transparent ROI mask.
- Fallback: upload a still image and OCR the ROI.

## D) Run instructions

```bash
npm run dev
```

Then open http://localhost:3000

- Grant camera permission and select a camera from the dropdown.
- Adjust ROI sliders so the white band is within the green rectangle.
- Watch codes appear. Edit the textarea if necessary.
- Click "Save to File" (writes `data/codes.txt`) or "Download .txt".

If server writes fail (e.g., serverless), the app still supports local download.

## E) Manual tests

- Use "Pause & OCR still" or upload a mock image to test without camera motion.
- Try dim/bright lighting; verify thresholding still extracts text.
- Confirm deduplication (case-insensitive) and newline appends.
- Verify 2-second lockout prevents flicker duplicates; Capture always bypasses.
- With no available camera, ensure the upload fallback works.

## Notes

- Tesseract worker loads `eng` language in the browser; no external OCR APIs required.
- Character-map heuristics nudge common ambiguities (O/0, I/1, !/1, $/S).
- ROI settings and codes mirror to localStorage and URL params for resilience.
