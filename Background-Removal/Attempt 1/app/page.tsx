"use client";

import React from "react";

type RemovalModule = typeof import("@imgly/background-removal");

export default function Page() {
  const [file, setFile] = React.useState<File | null>(null);
  const [originalUrl, setOriginalUrl] = React.useState<string>("");
  const [resultUrl, setResultUrl] = React.useState<string>("");
  const [resultBlob, setResultBlob] = React.useState<Blob | null>(null);
  const [loading, setLoading] = React.useState(false);
  const [error, setError] = React.useState<string>("");

  const inputRef = React.useRef<HTMLInputElement | null>(null);
  const dropRef = React.useRef<HTMLDivElement | null>(null);

  React.useEffect(() => {
    return () => {
      if (originalUrl) URL.revokeObjectURL(originalUrl);
      if (resultUrl) URL.revokeObjectURL(resultUrl);
    };
  }, [originalUrl, resultUrl]);

  const onBrowseClick = () => inputRef.current?.click();

  const onFilesSelected = async (files: FileList | null) => {
    if (!files || files.length === 0) return;
    const f = files[0];
    if (!f.type.startsWith("image/")) {
      setError("Please select an image file (PNG, JPG, WEBP).");
      return;
    }
    setError("");
    setFile(f);
    if (originalUrl) URL.revokeObjectURL(originalUrl);
    const url = URL.createObjectURL(f);
    setOriginalUrl(url);
    setResultUrl("");
    setResultBlob(null);
    await runRemoval(f);
  };

  const onDrop = async (e: React.DragEvent<HTMLDivElement>) => {
    e.preventDefault();
    e.stopPropagation();
    e.currentTarget.classList.remove("dragover");
    await onFilesSelected(e.dataTransfer.files);
  };

  const onDragOver = (e: React.DragEvent<HTMLDivElement>) => {
    e.preventDefault();
    e.stopPropagation();
    e.currentTarget.classList.add("dragover");
  };

  const onDragLeave = (e: React.DragEvent<HTMLDivElement>) => {
    e.preventDefault();
    e.stopPropagation();
    e.currentTarget.classList.remove("dragover");
  };

  async function runRemoval(targetFile?: File) {
    const imgFile = targetFile ?? file;
    if (!imgFile) return;

    setLoading(true);
    setError("");

    try {
      // Dynamic import in the browser only
      const mod: RemovalModule = await import("@imgly/background-removal");

      // Prefer removeBackground if available, else fall back to createWorker
      let blob: Blob | null = null;
      if (typeof (mod as any).removeBackground === "function") {
        blob = await (mod as any).removeBackground(imgFile, {
          output: { format: "image/png" },
          // progress: (key: string, current: number, total: number) => {}
        });
      } else if (typeof (mod as any).createWorker === "function") {
        const worker = await (mod as any).createWorker();
        blob = await worker(imgFile, { output: { format: "image/png" } });
      } else {
        throw new Error(
          "@imgly/background-removal API not found. Check package version."
        );
      }

      if (!blob) throw new Error("No result blob produced.");

      if (resultUrl) URL.revokeObjectURL(resultUrl);
      const url = URL.createObjectURL(blob);
      setResultUrl(url);
      setResultBlob(blob);
    } catch (err: any) {
      console.error(err);
      setError(err?.message || "Failed to remove background.");
    } finally {
      setLoading(false);
    }
  }

  const onRetry = async () => {
    if (!file) return;
    await runRemoval(file);
  };

  const onDownload = () => {
    if (!resultBlob || !file) return;
    const a = document.createElement("a");
    const url = URL.createObjectURL(resultBlob);
    a.href = url;
    const base = file.name.replace(/\.[^/.]+$/, "");
    a.download = `${base}-bg-removed.png`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  };

  return (
    <div className="container">
      <header className="header">
        <h1>Background Removal – Attempt 1</h1>
        <p>Drag & drop an image. Preview the result. Retry or download.</p>
      </header>

      <div className="grid" style={{ marginBottom: 16 }}>
        <div className="card">
          <h3>Upload</h3>
          <div className="body">
            <div
              className="dropzone"
              ref={dropRef}
              onClick={onBrowseClick}
              onDrop={onDrop}
              onDragOver={onDragOver}
              onDragLeave={onDragLeave}
            >
              <div>
                <strong>Drag & drop</strong> or click to browse
              </div>
              <div className="hint">PNG, JPG, WEBP up to ~10MB</div>
            </div>
            <input
              ref={inputRef}
              type="file"
              accept="image/*"
              style={{ display: "none" }}
              onChange={(e) => onFilesSelected(e.target.files)}
            />
            <div className="actions">
              <button className="button" onClick={onBrowseClick}>Browse</button>
              <button className="button" onClick={() => setFile(null)} disabled={!file}>
                Clear
              </button>
            </div>
          </div>
        </div>

        <div className="card">
          <h3>Original</h3>
          <div className="body">
            <div className="image-wrap" style={{ minHeight: 220 }}>
              {originalUrl ? (
                <img src={originalUrl} alt="Original" />
              ) : (
                <span className="small">No image selected</span>
              )}
            </div>
          </div>
        </div>
      </div>

      <div className="grid">
        <div className="card">
          <h3>Result</h3>
          <div className="body">
            <div className="image-wrap">
              {loading ? (
                <span className="small">Processing…</span>
              ) : resultUrl ? (
                <img src={resultUrl} alt="Result" />
              ) : (
                <span className="small">Result will appear here</span>
              )}
            </div>
            <div className="actions">
              <button className="button" onClick={onRetry} disabled={!file || loading}>
                Retry
              </button>
              <button
                className="button primary"
                onClick={onDownload}
                disabled={!resultBlob || loading}
              >
                Download
              </button>
            </div>
            {error && <div className="status error">{error}</div>}
            {!error && resultUrl && (
              <div className="status success">Done. You can download the PNG.</div>
            )}
          </div>
        </div>

        <div className="card">
          <h3>Notes</h3>
          <div className="body">
            <ul>
              <li className="small">
                Processing happens in your browser via <code>@imgly/background-removal</code>.
              </li>
              <li className="small">
                If models fail to load, ensure network access or consult the package docs about asset paths.
              </li>
            </ul>
          </div>
        </div>
      </div>
    </div>
  );
}
