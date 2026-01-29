"use client";

import { createWorker, type Worker, PSM } from 'tesseract.js'

export type OcrResult = { text: string; normalized: string | null; confidence: number }

let worker: Worker | null = null
let workerInit: Promise<void> | null = null

async function ensureWorker() {
  if (worker && workerInit) { await workerInit; return worker }
  worker = await createWorker()
  workerInit = (async () => {
    await worker!.loadLanguage('eng')
    await worker!.initialize('eng')
    // Set parameters: whitelist and single line mode
    await worker!.setParameters({
      tessedit_char_whitelist: 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789',
      tessedit_pageseg_mode: PSM.SINGLE_LINE,
      preserve_interword_spaces: '1',
      user_defined_dpi: '300',
      load_system_dawg: '0',
      load_freq_dawg: '0'
    })
  })()
  await workerInit
  return worker!
}

export function preprocessCanvas(input: HTMLCanvasElement) {
  const w = input.width, h = input.height

  // 1) Read source to RGBA and compute grayscale + histogram
  const srcCtx = input.getContext('2d')!
  const src = srcCtx.getImageData(0, 0, w, h)
  const d = src.data
  const gray = new Uint8ClampedArray(w * h)
  const hist = new Uint32Array(256)
  for (let i = 0, j = 0; i < d.length; i += 4, j++) {
    const y = Math.round(0.2126 * d[i] + 0.7152 * d[i + 1] + 0.0722 * d[i + 2])
    gray[j] = y
    hist[y]++
  }

  // 2) Auto-contrast via percentile stretch (2% - 98%)
  const total = w * h
  const lowCount = Math.floor(total * 0.02)
  const highCount = Math.floor(total * 0.98)
  let cum = 0
  let lo = 0, hi = 255
  for (let i = 0; i < 256; i++) { cum += hist[i]; if (cum >= lowCount) { lo = i; break } }
  cum = 0
  for (let i = 255; i >= 0; i--) { cum += hist[i]; if (cum >= total - highCount) { hi = i; break } }
  const span = Math.max(10, hi - lo)
  for (let i = 0; i < gray.length; i++) {
    const v = Math.max(0, Math.min(255, Math.round((gray[i] - lo) * 255 / span)))
    gray[i] = v
  }

  // 3) Otsu threshold
  const otsu = (() => {
    // recompute hist for stretched gray
    const hist2 = new Uint32Array(256)
    for (let i = 0; i < gray.length; i++) hist2[gray[i]]++
    let sum = 0
    for (let t = 0; t < 256; t++) sum += t * hist2[t]
    let sumB = 0, wB = 0, wF = 0
    let varMax = 0
    let threshold = 127
    for (let t = 0; t < 256; t++) {
      wB += hist2[t]
      if (wB === 0) continue
      wF = total - wB
      if (wF === 0) break
      sumB += t * hist2[t]
      const mB = sumB / wB
      const mF = (sum - sumB) / wF
      const between = wB * wF * (mB - mF) * (mB - mF)
      if (between > varMax) { varMax = between; threshold = t }
    }
    return threshold
  })()

  // 4) Binarize with Otsu, ensure black text on white background
  const bin = new Uint8ClampedArray(w * h)
  for (let i = 0; i < gray.length; i++) bin[i] = gray[i] > otsu ? 255 : 0

  // 5) Light dilation (3x3) to thicken strokes
  const dil = new Uint8ClampedArray(w * h)
  const offsets = [-w - 1, -w, -w + 1, -1, 0, 1, w - 1, w, w + 1]
  for (let y = 1; y < h - 1; y++) {
    for (let x = 1; x < w - 1; x++) {
      const idx = y * w + x
      let v = 0
      for (const off of offsets) { if (bin[idx + off] === 0) { v = 0; break } v = 255 }
      // Keep white background, black text = 0; we invert after upscale for Tesseract
      dil[idx] = v === 255 ? 255 : 0
    }
  }

  // 6) Upscale for better OCR
  const scale = Math.max(2, Math.ceil(480 / h)) // target ROI height ~480px
  const outW = Math.max(w * scale, 400)
  const outH = Math.max(h * scale, 200)
  const out = document.createElement('canvas')
  out.width = outW; out.height = outH
  const octx = out.getContext('2d')!
  // Draw from binary into an intermediate canvas, then scale without smoothing
  const mid = document.createElement('canvas'); mid.width = w; mid.height = h
  const midCtx = mid.getContext('2d')!
  const midImg = midCtx.createImageData(w, h)
  for (let i = 0, j = 0; i < midImg.data.length; i += 4, j++) {
    const v = dil[j]
    // invert to black text (0) on white (255)? Our dilation used white for bg, keep white bg
    midImg.data[i] = v
    midImg.data[i + 1] = v
    midImg.data[i + 2] = v
    midImg.data[i + 3] = 255
  }
  midCtx.putImageData(midImg, 0, 0)
  octx.imageSmoothingEnabled = false
  octx.drawImage(mid, 0, 0, outW, outH)

  return out
}

function mapAmbiguous(input: string): string {
  // Simple glyph nudges for OCR ambiguities
  return input
    .replace(/[|!]/g, '1')
    .replace(/[o]/g, 'O')
    .replace(/\$/g, 'S')
}

export function normalizeCode(raw: string): string | null {
  // Normalize to uppercase alphanumerics plus spaces
  const up = mapAmbiguous(raw).toUpperCase().replace(/[^A-Z0-9\s]/g, ' ').replace(/\s+/g, ' ').trim()

  // We expect exactly 16 alphanumerics as 4 groups of 4
  const spaced4x4 = /\b([A-Z0-9]{4})\s+([A-Z0-9]{4})\s+([A-Z0-9]{4})\s+([A-Z0-9]{4})\b/
  const contiguous16 = /\b([A-Z0-9]{16})\b/

  const candidates: string[] = []
  const tryVariants = [
    up,
    up.replace(/O/g, '0').replace(/I/g, '1'), // coerce letters to digits where ambiguous
    up.replace(/0/g, 'O').replace(/1/g, 'I'), // coerce digits to letters where ambiguous
  ]

  for (const s of tryVariants) {
    const m = s.match(spaced4x4)
    if (m) {
      const code = `${m[1]} ${m[2]} ${m[3]} ${m[4]}`
      candidates.push(code)
    }
    const c = s.match(contiguous16)
    if (c) {
      const compact = c[1]
      candidates.push(`${compact.slice(0,4)} ${compact.slice(4,8)} ${compact.slice(8,12)} ${compact.slice(12,16)}`)
    }
  }

  // Prefer the first candidate found
  const first = candidates.find(Boolean)
  return first ?? null
}

export async function recognizeCanvas(canvas: HTMLCanvasElement): Promise<OcrResult> {
  const w = await ensureWorker()
  // Build variants from the preprocessed input
  const variants: HTMLCanvasElement[] = []
  variants.push(canvas)
  variants.push(invertCanvas(canvas))
  variants.push(upscaleCanvas(canvas, 1.5))

  let best: OcrResult = { text: '', normalized: null, confidence: 0 }
  for (const cv of variants) {
    const { data } = await w.recognize(cv)
    const text = data.text?.trim() ?? ''
    const normalized = normalizeCode(text)
    const confidence = normalized ? computeCharConfidence(data as any, normalized) : (Number.isFinite((data as any).confidence) ? (data as any).confidence : 0)
    const cand: OcrResult = { text, normalized, confidence }
    if (cand.confidence > best.confidence || (cand.normalized && !best.normalized)) {
      best = cand
    }
    // Early exit if we have a good normalized candidate
    if (best.normalized && best.confidence >= 70) break
  }
  return best
}

function computeCharConfidence(data: any, normalized: string): number {
  try {
    const target = normalized.replace(/\s+/g, '')
    const symbols: { text: string; confidence: number }[] = []
    if (Array.isArray(data?.symbols)) {
      for (const s of data.symbols) {
        const t = String(s.text || '')
        if (/^[A-Za-z0-9]$/.test(t)) symbols.push({ text: t.toUpperCase(), confidence: Number(s.confidence) || 0 })
      }
    } else if (Array.isArray(data?.words)) {
      for (const w of data.words) {
        const t = String(w.text || '')
        for (const ch of t.split('')) if (/^[A-Za-z0-9]$/.test(ch)) symbols.push({ text: ch.toUpperCase(), confidence: Number(w.confidence) || 0 })
      }
    }
    if (symbols.length === 0) return Number.isFinite(data?.confidence) ? data.confidence : 0
    const seq = symbols.map(s => s.text).join('')
    let bestAvg = 0
    for (let i = 0; i + target.length <= seq.length; i++) {
      let sum = 0, cnt = 0
      for (let k = 0; k < target.length; k++) {
        if (seq[i + k] === target[k]) { sum += symbols[i + k].confidence; cnt++ } else { sum += 0; cnt++ }
      }
      const avg = cnt ? (sum / cnt) : 0
      if (avg > bestAvg) bestAvg = avg
    }
    // If we couldn't find an aligned window, fall back
    if (bestAvg === 0) return Number.isFinite(data?.confidence) ? data.confidence : 0
    return bestAvg
  } catch {
    return Number.isFinite(data?.confidence) ? data.confidence : 0
  }
}

function invertCanvas(input: HTMLCanvasElement): HTMLCanvasElement {
  const out = document.createElement('canvas')
  out.width = input.width; out.height = input.height
  const ictx = input.getContext('2d')!
  const octx = out.getContext('2d')!
  const img = ictx.getImageData(0, 0, input.width, input.height)
  const d = img.data
  for (let i = 0; i < d.length; i += 4) {
    d[i] = 255 - d[i]
    d[i + 1] = 255 - d[i + 1]
    d[i + 2] = 255 - d[i + 2]
    d[i + 3] = 255
  }
  octx.putImageData(img, 0, 0)
  return out
}

function upscaleCanvas(input: HTMLCanvasElement, factor: number): HTMLCanvasElement {
  const out = document.createElement('canvas')
  out.width = Math.max(1, Math.round(input.width * factor))
  out.height = Math.max(1, Math.round(input.height * factor))
  const octx = out.getContext('2d')!
  octx.imageSmoothingEnabled = false
  octx.drawImage(input, 0, 0, out.width, out.height)
  return out
}
