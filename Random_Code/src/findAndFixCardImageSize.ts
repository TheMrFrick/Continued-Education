import * as fs from 'fs';
import * as path from 'path';
import sharp from 'sharp';

const TARGET_WIDTH = 1464;
const TARGET_HEIGHT = 2042;
const ALLOWED_DIMENSIONS = [
  { width: 1464, height: 2042 },
  { width: 2042, height: 1464 }
];

async function getImageDimensions(imagePath: string): Promise<{ width: number; height: number } | null> {
  try {
    const metadata = await sharp(imagePath).metadata();
    if (metadata.width && metadata.height) {
      return { width: metadata.width, height: metadata.height };
    }
    return null;
  } catch {
    return null;
  }
}

async function cropImage(inputPath: string, outputPath: string, sourceWidth: number, sourceHeight: number): Promise<void> {
  const cropLeft = Math.round((sourceWidth - TARGET_WIDTH) / 2);
  const cropTop = Math.round((sourceHeight - TARGET_HEIGHT) / 2);
  
  await sharp(inputPath)
    .extract({ left: cropLeft, top: cropTop, width: TARGET_WIDTH, height: TARGET_HEIGHT })
    .toFile(outputPath);
}

async function processDirectory(directoryPath: string): Promise<void> {
  const outDir = path.join(directoryPath, 'out');
  
  if (!fs.existsSync(outDir)) {
    fs.mkdirSync(outDir, { recursive: true });
  }

  const files = fs.readdirSync(directoryPath);
  const imageExtensions = new Set(['.png', '.jpg', '.jpeg', '.webp', '.gif', '.bmp', '.tiff']);

  const imageFiles = files.filter(file => {
    const ext = path.extname(file).toLowerCase();
    return imageExtensions.has(ext) && fs.statSync(path.join(directoryPath, file)).isFile();
  });

  console.log(`Found ${imageFiles.length} image(s) in ${directoryPath}`);

  for (const file of imageFiles) {
    const inputPath = path.join(directoryPath, file);
    const outputPath = path.join(outDir, file);

    const dimensions = await getImageDimensions(inputPath);
    
    if (!dimensions) {
      console.log(`  ⚠️  ${file}: Could not read dimensions`);
      continue;
    }

    const { width, height } = dimensions;
    const isCorrectSize = ALLOWED_DIMENSIONS.some(
      d => d.width === width && d.height === height
    );

    if (isCorrectSize) {
      console.log(`  ✓  ${file}: ${width}x${height} (correct size, skipping)`);
    } else {
      console.log(`  →  ${file}: ${width}x${height} → ${TARGET_WIDTH}x${TARGET_HEIGHT} (cropping)`);
      await cropImage(inputPath, outputPath, width, height);
    }
  }

  console.log(`\nDone! Output written to: ${outDir}`);
}

// Get directory from command line arguments
const targetDir = process.argv[2];

if (!targetDir) {
  console.error('Usage: npx ts-node findAndFixCardImageSize.ts <directory>');
  process.exit(1);
}

const resolvedDir = path.resolve(targetDir);

if (!fs.existsSync(resolvedDir)) {
  console.error(`Error: Directory does not exist: ${resolvedDir}`);
  process.exit(1);
}

if (!fs.statSync(resolvedDir).isDirectory()) {
  console.error(`Error: Not a directory: ${resolvedDir}`);
  process.exit(1);
}

processDirectory(resolvedDir).catch(err => {
  console.error('Error:', err);
  process.exit(1);
});
