"use client";

import React, { useMemo } from 'react'
import { uniqueCaseInsensitive } from '@/lib/storage'

interface Props {
  value: string
  onChange: (val: string) => void
  onClear: () => void
  onSave: () => void
  onDownload: () => void
}

export default function DetectedTextArea({ value, onChange, onClear, onSave, onDownload }: Props) {
  const lineCount = useMemo(() => value.split(/\r?\n/).filter(Boolean).length, [value])
  const dedupPreview = useMemo(() => uniqueCaseInsensitive(value.split(/\r?\n/).filter(Boolean)).join('\n'), [value])

  return (
    <div className="space-y-2">
      <div className="flex items-center justify-between">
        <label className="label">Detected Codes ({lineCount})</label>
        <div className="flex gap-2">
          <button className="btn" onClick={onClear} aria-label="Clear textarea">Clear</button>
          <button className="btn" onClick={onSave} aria-label="Save to server file">Save to File</button>
          <button className="btn" onClick={onDownload} aria-label="Download text file">Download .txt</button>
        </div>
      </div>
      <textarea
        aria-label="Detected codes"
        className="w-full h-[420px] rounded-md border border-gray-300 dark:border-gray-700 bg-white dark:bg-gray-900 p-2 font-mono text-sm"
        value={value}
        onChange={(e) => onChange(e.target.value)}
        placeholder="Detected codes will appear here...\nYou can edit this at any time."
      />
      <div className="text-xs text-gray-500">Preview (deduped):
        <pre className="mt-1 p-2 bg-gray-100 dark:bg-gray-800 rounded border border-gray-200 dark:border-gray-700 overflow-auto max-h-40">{dedupPreview}</pre>
      </div>
    </div>
  )
}
