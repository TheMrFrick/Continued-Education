import fs from 'fs';
import path from 'path';
import sharp from 'sharp';

const INPUT_DIR = 'E:\\Deckplanet Data\\Neuroscape\\Card Images';
const OUTPUT_DIR = path.join(INPUT_DIR, 'output');

async function processImages(): Promise<void> {
  // Ensure output directory exists
  if (!fs.existsSync(OUTPUT_DIR)) {
    fs.mkdirSync(OUTPUT_DIR, { recursive: true });
  }

  // Get all PNG files
  const files = fs.readdirSync(INPUT_DIR);
  const pngFiles = files.filter(file => file.toLowerCase().endsWith('.png'));

  console.log(`Found ${pngFiles.length} PNG files`);

  for (const file of pngFiles) {
    const inputPath = path.join(INPUT_DIR, file);
    
    // Check if filename starts with 3 numbers
    const match = file.match(/^(\d{3})(.*)$/);
    let outputName: string;
    
    if (match) {
      // Rename to GEN_###.webp
      const numberPart = match[1];
      outputName = `GEN_${numberPart}.webp`;
    } else {
      // Keep original name but change extension
      outputName = path.parse(file).name + '.webp';
    }

    const outputPath = path.join(OUTPUT_DIR, outputName);

    try {
      await sharp(inputPath)
        .webp({ quality: 85 })
        .toFile(outputPath);
      
      console.log(`Converted: ${file} -> ${outputName}`);
    } catch (error) {
      console.error(`Error converting ${file}:`, error);
    }
  }

  console.log('Conversion complete!');
}

processImages();
