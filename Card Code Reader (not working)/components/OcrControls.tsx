"use client";

import React from 'react'

interface Props {
  roiTopPct: number
  roiHeightPct: number
  setRoiTopPct: (n: number) => void
  setRoiHeightPct: (n: number) => void
  autoCapture: boolean
  setAutoCapture: (b: boolean) => void
  lastConfidence: number | null
}

export default function OcrControls({ roiTopPct, roiHeightPct, setRoiTopPct, setRoiHeightPct, autoCapture, setAutoCapture, lastConfidence }: Props) {
  return (
    <div className="grid grid-cols-1 sm:grid-cols-3 gap-4 p-3 rounded-md border border-gray-200 bg-white dark:bg-gray-800 dark:border-gray-700">
      <div>
        <label className="label" htmlFor="top">ROI Top: {Math.round(roiTopPct)}%</label>
        <input id="top" aria-label="ROI top percent" className="w-full" type="range" min={0} max={80} step={1}
               value={roiTopPct} onChange={(e) => setRoiTopPct(Number(e.target.value))} />
      </div>
      <div>
        <label className="label" htmlFor="height">ROI Height: {Math.round(roiHeightPct)}%</label>
        <input id="height" aria-label="ROI height percent" className="w-full" type="range" min={10} max={60} step={1}
               value={roiHeightPct} onChange={(e) => setRoiHeightPct(Number(e.target.value))} />
      </div>
      <div className="flex flex-col justify-between gap-2">
        <label className="inline-flex items-center gap-2">
          <input aria-label="Toggle auto capture" type="checkbox" checked={autoCapture} onChange={(e) => setAutoCapture(e.target.checked)} />
          <span className="label">Auto-capture</span>
        </label>
        <div className="text-sm text-gray-600 dark:text-gray-300">Confidence: {lastConfidence !== null ? Math.round(lastConfidence) + '%' : '—'}</div>
      </div>
    </div>
  )
}
