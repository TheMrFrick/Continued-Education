declare module '@imgly/background-removal' {
  export function createWorker(): Promise<(
    input: Blob | File | ArrayBuffer | ImageData | string,
    options?: any
  ) => Promise<Blob>>;

  export function removeBackground(
    input: Blob | File | ArrayBuffer | ImageData | string,
    options?: any
  ): Promise<Blob>;
}
