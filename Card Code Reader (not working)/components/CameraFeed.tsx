"use client";

import React, { forwardRef, useEffect, useImperativeHandle, useRef, useState } from 'react'

export type CameraFeedHandle = {
  getRoiCanvas: () => HTMLCanvasElement | null
}

interface CameraFeedProps {
  deviceId?: string
  onDeviceChange: (id: string) => void
  roiTopPct: number
  roiHeightPct: number
}

const CameraFeed = forwardRef<CameraFeedHandle, CameraFeedProps>(function CameraFeed(
  { deviceId, onDeviceChange, roiTopPct, roiHeightPct }, ref
) {
  const videoRef = useRef<HTMLVideoElement | null>(null)
  const canvasRef = useRef<HTMLCanvasElement | null>(null) // offscreen ROI
  const [devices, setDevices] = useState<MediaDeviceInfo[]>([])
  const [error, setError] = useState<string | null>(null)
  const [hasStream, setHasStream] = useState(false)
  const streamRef = useRef<MediaStream | null>(null)

  // enumerate devices
  useEffect(() => {
    async function loadDevices() {
      try {
        if (!navigator.mediaDevices?.enumerateDevices) throw new Error('enumerateDevices not supported')
        const devs = await navigator.mediaDevices.enumerateDevices()
        setDevices(devs.filter(d => d.kind === 'videoinput'))
      } catch (e: any) {
        setError(e?.message ?? 'Unable to enumerate devices')
      }
    }
    loadDevices()
  }, [])

  // start stream on device change
  useEffect(() => {
    let currentStream: MediaStream | null = null
    async function start() {
      setError(null)
      try {
        if (!navigator.mediaDevices?.getUserMedia) throw new Error('getUserMedia not supported')
        const constraints: MediaStreamConstraints = {
          video: {
            deviceId: deviceId ? { exact: deviceId } : undefined,
            facingMode: deviceId ? undefined : { ideal: 'environment' },
            width: { ideal: 1920 },
            height: { ideal: 1080 },
            frameRate: { ideal: 30 },
          },
          audio: false,
        }
        const stream = await navigator.mediaDevices.getUserMedia(constraints)
        currentStream = stream
        streamRef.current = stream
        // Try to apply continuous focus/exposure/white balance if supported
        try {
          const track = stream.getVideoTracks()[0]
          // Some browsers support these advanced constraints; not all are typed in TS DOM lib
          await (track as any).applyConstraints({ advanced: [ { focusMode: 'continuous' }, { exposureMode: 'continuous' }, { whiteBalanceMode: 'continuous' } ] })
        } catch {}
        if (videoRef.current) {
          videoRef.current.srcObject = stream
          await videoRef.current.play()
          setHasStream(true)
        }
      } catch (e: any) {
        setError(e?.message ?? 'Failed to open camera')
        setHasStream(false)
      }
    }

    start()

    return () => {
      if (currentStream) {
        currentStream.getTracks().forEach(t => t.stop())
      }
    }
  }, [deviceId])

  useImperativeHandle(ref, () => ({
    getRoiCanvas: () => {
      // Prefer high-resolution still via ImageCapture if available
      const stream = streamRef.current
      const VideoCtor: any = (globalThis as any)
      const ImageCaptureCtor = (globalThis as any).ImageCapture
      const computeCrop = (w: number, h: number) => {
        const top = Math.max(0, Math.min(100, roiTopPct)) / 100 * h
        const height = Math.max(5, Math.min(100, roiHeightPct)) / 100 * h
        const cw = w
        const ch = Math.min(h - top, height)
        return { sx: 0, sy: top, sw: w, sh: ch, dw: cw, dh: ch }
      }

      const cnv = canvasRef.current || document.createElement('canvas')
      const video = videoRef.current

      const tryVideo = () => {
        if (!video) return null
        const vw = video.videoWidth
        const vh = video.videoHeight
        if (!vw || !vh) return null
        const { sx, sy, sw, sh, dw, dh } = computeCrop(vw, vh)
        cnv.width = Math.max(1, dw)
        cnv.height = Math.max(1, dh)
        const ctx = cnv.getContext('2d')!
        ctx.drawImage(video, sx, sy, sw, sh, 0, 0, dw, dh)
        canvasRef.current = cnv
        return cnv
      }

      if (stream && ImageCaptureCtor) {
        try {
          const track = stream.getVideoTracks()[0]
          const ic = new (ImageCaptureCtor as any)(track)
          // Note: this is async; we can't make getRoiCanvas async, so we kick off a sync grab attempt
          // To keep API sync, we do a blocking trick using a synchronous fallback and schedule an async update.
          const fallback = tryVideo()
          ;(async () => {
            try {
              const bmp: ImageBitmap = await ic.grabFrame()
              const { width: bw, height: bh } = bmp
              const { sx, sy, sw, sh, dw, dh } = computeCrop(bw, bh)
              cnv.width = Math.max(1, dw)
              cnv.height = Math.max(1, dh)
              const ctx = cnv.getContext('2d')!
              ctx.drawImage(bmp, sx, sy, sw, sh, 0, 0, dw, dh)
              canvasRef.current = cnv
            } catch {}
          })()
          return fallback
        } catch {
          return tryVideo()
        }
      }

      return tryVideo()
    }
  }), [roiTopPct, roiHeightPct])

  const handleSelect = (e: React.ChangeEvent<HTMLSelectElement>) => {
    onDeviceChange(e.target.value)
  }

  return (
    <div className="space-y-2">
      <div className="flex items-center gap-2">
        <label className="label" htmlFor="camera">Camera</label>
        <select id="camera" className="input min-w-[240px]" value={deviceId || ''} onChange={handleSelect} aria-label="Select camera">
          <option value="">Default (environment)</option>
          {devices.map((d, idx) => (
            <option key={d.deviceId || idx} value={d.deviceId}>{d.label || `Camera ${idx + 1}`}</option>
          ))}
        </select>
      </div>

      {error && (
        <div className="p-3 rounded-md bg-red-50 text-red-700 border border-red-200">
          <p className="font-medium">{error}</p>
          <p className="text-sm">No camera? Upload a still image below in the main panel to OCR.</p>
        </div>
      )}

      <div className="roi-container">
        <video ref={videoRef} playsInline muted className="w-full rounded-md border border-gray-200 bg-black" />
        {/* ROI overlay */}
        <div
          className="roi-clip"
          style={{ top: `${roiTopPct}%`, height: `${roiHeightPct}%` }}
          aria-hidden
        />
      </div>
    </div>
  )
})

export default CameraFeed
