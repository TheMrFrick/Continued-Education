"use client";

import { useCallback, useEffect, useRef, useState } from "react";
import { usePathname, useRouter, useSearchParams } from "next/navigation";
import CameraFeed, { CameraFeedHandle } from "@/components/CameraFeed";
import OcrControls from "@/components/OcrControls";
import DetectedTextArea from "@/components/DetectedTextArea";
import { preprocessCanvas, recognizeCanvas, normalizeCode, type OcrResult } from "@/lib/ocr";
import { downloadText, getJSON, setJSON, uniqueCaseInsensitive } from "@/lib/storage";

const ROI_TOP_KEY = "roi_top_pct";
const ROI_HEIGHT_KEY = "roi_height_pct";
const CAMERA_KEY = "camera_device_id";
const TEXTAREA_KEY = "codes_textarea";

export default function Page() {
  const router = useRouter();
  const pathname = usePathname();
  const searchParams = useSearchParams();

  const cameraRef = useRef<CameraFeedHandle | null>(null);
  const uploadImgRef = useRef<HTMLImageElement | null>(null);

  const [running, setRunning] = useState(false);
  const [autoCapture, setAutoCapture] = useState(true);
  const [deviceId, setDeviceId] = useState<string | undefined>(undefined);

  const [roiTopPct, setRoiTopPct] = useState<number>(() => {
    const sp = searchParams?.get("top");
    const ls = typeof window !== 'undefined' ? getJSON<number>(ROI_TOP_KEY) : null;
    return sp ? Number(sp) : ls ?? 35;
  });
  const [roiHeightPct, setRoiHeightPct] = useState<number>(() => {
    const sp = searchParams?.get("h");
    const ls = typeof window !== 'undefined' ? getJSON<number>(ROI_HEIGHT_KEY) : null;
    return sp ? Number(sp) : ls ?? 22;
  });

  const [lastResult, setLastResult] = useState<OcrResult | null>(null);
  const [lastTimestamp, setLastTimestamp] = useState<string>("");
  const [lockout, setLockout] = useState(false);
  const [recent, setRecent] = useState<string[]>([]); // last N

  const [textArea, setTextArea] = useState<string>(() => {
    if (typeof window === 'undefined') return "";
    return getJSON<string>(TEXTAREA_KEY) ?? "";
  });

  // Upload fallback state
  const [uploadUrl, setUploadUrl] = useState<string | null>(null);

  // persist camera and ROI and textarea
  useEffect(() => { setJSON(TEXTAREA_KEY, textArea); }, [textArea]);
  useEffect(() => { setJSON(CAMERA_KEY, deviceId); }, [deviceId]);
  useEffect(() => { setJSON(ROI_TOP_KEY, roiTopPct); setJSON(ROI_HEIGHT_KEY, roiHeightPct); }, [roiTopPct, roiHeightPct]);
  useEffect(() => {
    const sp = new URLSearchParams(searchParams ?? undefined);
    sp.set('top', String(roiTopPct));
    sp.set('h', String(roiHeightPct));
    router.replace(`${pathname}?${sp.toString()}`, { scroll: false });
  }, [roiTopPct, roiHeightPct]);

  // hydrate camera selection
  useEffect(() => {
    const saved = getJSON<string>(CAMERA_KEY);
    if (saved) setDeviceId(saved);
  }, []);

  const appendCode = useCallback((code: string, force = false, conf = 0) => {
    setTextArea((prev: string) => {
      const lines = prev.split(/\r?\n/).filter(Boolean);
      const N = 5;
      const recentWindow = lines.slice(-N);
      const isDupRecent = recentWindow.some((l: string) => l.trim().toUpperCase() === code.toUpperCase());
      const isDupAny = lines.some((l: string) => l.trim().toUpperCase() === code.toUpperCase());
      if (!force && (isDupRecent || isDupAny)) return prev; // dedupe
      const nextLines = [...lines, code];
      return nextLines.join("\n");
    });
    setRecent((r: string[]) => {
      const next = [...r, code].slice(-5);
      return next;
    });
    setLastTimestamp(new Date().toLocaleTimeString());
    if (!force) {
      setLockout(true);
      setTimeout(() => setLockout(false), 2000);
    }
  }, []);

  const handleOcrOnce = useCallback(async (force = false) => {
    const roiCanvas = await cameraRef.current?.getRoiCanvas();
    if (!roiCanvas) return;
    const processed = preprocessCanvas(roiCanvas);
    const result = await recognizeCanvas(processed);
    const normalized = result.normalized ?? normalizeCode(result.text);
    const finalRes: OcrResult = {
      text: result.text,
      normalized,
      confidence: result.confidence,
    };
    setLastResult(finalRes);
    setLastTimestamp(new Date().toLocaleTimeString());
    if ((force || (autoCapture && finalRes.normalized && finalRes.confidence >= 70)) && finalRes.normalized) {
      if (!lockout || force) appendCode(finalRes.normalized, force, finalRes.confidence);
    }
  }, [autoCapture, lockout, appendCode]);

  const handleOcrUpload = useCallback(async () => {
    if (!uploadImgRef.current) return;
    const img = uploadImgRef.current;
    if (!img.naturalWidth || !img.naturalHeight) return;
    const iw = img.naturalWidth;
    const ih = img.naturalHeight;
    const top = Math.max(0, Math.min(100, roiTopPct)) / 100 * ih;
    const height = Math.max(5, Math.min(100, roiHeightPct)) / 100 * ih;
    const ch = Math.min(ih - top, height);
    const cnv = document.createElement('canvas');
    cnv.width = iw; cnv.height = ch;
    const ctx = cnv.getContext('2d')!;
    ctx.drawImage(img, 0, top, iw, ch, 0, 0, iw, ch);
    const processed = preprocessCanvas(cnv);
    const result = await recognizeCanvas(processed);
    const normalized = result.normalized ?? normalizeCode(result.text);
    const finalRes: OcrResult = { text: result.text, normalized, confidence: result.confidence };
    setLastResult(finalRes);
    setLastTimestamp(new Date().toLocaleTimeString());
    if (finalRes.normalized) appendCode(finalRes.normalized, true, finalRes.confidence);
  }, [roiTopPct, roiHeightPct, appendCode]);

  // Stream loop every 750ms while running
  useEffect(() => {
    if (!running) return;
    let stop = false;
    const tick = async () => {
      if (stop) return;
      await handleOcrOnce(false);
      if (stop) return;
      setTimeout(tick, 750);
    };
    tick();
    return () => { stop = true; };
  }, [running, handleOcrOnce]);

  const onClear = () => setTextArea("");

  const onSave = async () => {
    try {
      const content = uniqueCaseInsensitive(textArea.split(/\r?\n/).filter(Boolean)).join("\n") + "\n";
      const res = await fetch("/api/save", { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ content }) });
      const data = await res.json();
      if (!res.ok || !data.ok) throw new Error(data.error || "Save failed");
      alert("Saved to server file: data/codes.txt");
    } catch (e) {
      console.error(e);
      // fallback to download
      downloadText("codes.txt", textArea.trim() + "\n");
    }
  };

  const onDownload = () => {
    const content = uniqueCaseInsensitive(textArea.split(/\r?\n/).filter(Boolean)).join("\n");
    downloadText("codes.txt", content + (content.endsWith("\n") ? "" : "\n"));
  };

  return (
    <main className="space-y-6">
      <h1 className="text-2xl font-semibold">Card Code Reader</h1>

      <section className="grid grid-cols-1 lg:grid-cols-5 gap-6">
        <div className="lg:col-span-3 space-y-4">
          <CameraFeed
            ref={cameraRef}
            deviceId={deviceId}
            onDeviceChange={setDeviceId}
            roiTopPct={roiTopPct}
            roiHeightPct={roiHeightPct}
          />

          <div className="flex flex-wrap gap-2">
            <button className="btn" aria-label={running ? "Stop" : "Start"} onClick={() => setRunning((r) => !r)}>
              {running ? "Stop" : "Start"}
            </button>
            <button className="btn" aria-label="Capture still" onClick={() => handleOcrOnce(true)}>Capture</button>
            <button className="btn" aria-label="Pause and OCR still frame" onClick={() => handleOcrOnce(true)}>Pause & OCR still</button>
          </div>

          <OcrControls
            roiTopPct={roiTopPct}
            roiHeightPct={roiHeightPct}
            setRoiTopPct={setRoiTopPct}
            setRoiHeightPct={setRoiHeightPct}
            autoCapture={autoCapture}
            setAutoCapture={setAutoCapture}
            lastConfidence={lastResult?.confidence ?? null}
          />

          <div className="text-sm text-gray-600 dark:text-gray-300">
            <div><span className="font-semibold">Last:</span> {lastResult?.normalized ?? "—"}</div>
            <div><span className="font-semibold">Confidence:</span> {lastResult ? `${Math.round(lastResult.confidence)}%` : "—"}</div>
            <div><span className="font-semibold">Timestamp:</span> {lastTimestamp || "—"}</div>
          </div>

          {/* File upload fallback */}
          <div className="space-y-2 p-3 rounded-md border border-gray-200 bg-white dark:bg-gray-800 dark:border-gray-700">
            <label className="label">No camera? Upload a still image to OCR</label>
            <input
              aria-label="Upload image for OCR"
              type="file"
              accept="image/*"
              onChange={(e: React.ChangeEvent<HTMLInputElement>) => {
                const f = e.target.files?.[0];
                if (!f) return;
                const url = URL.createObjectURL(f);
                setUploadUrl((prev: string | null) => { if (prev) URL.revokeObjectURL(prev); return url; });
              }}
              className="input"
            />
            {uploadUrl && (
              <div className="roi-container">
                <img ref={uploadImgRef} src={uploadUrl} alt="uploaded preview" className="max-h-[360px] w-auto rounded-md border border-gray-200" />
                <div className="roi-clip" style={{ top: `${roiTopPct}%`, height: `${roiHeightPct}%` }} />
              </div>
            )}
            <div>
              <button disabled={!uploadUrl} className="btn disabled:opacity-50" aria-label="OCR uploaded image" onClick={handleOcrUpload}>OCR Uploaded Image</button>
            </div>
          </div>
        </div>

        <div className="lg:col-span-2">
          <DetectedTextArea
            value={textArea}
            onChange={setTextArea}
            onClear={onClear}
            onSave={onSave}
            onDownload={onDownload}
          />
        </div>
      </section>
    </main>
  );
}
