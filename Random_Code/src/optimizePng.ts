import fs from "fs";
import sharp from "sharp";
import path from "path";

// Change this to the folder that contains the PNG files you want to optimize
const directoryPath =
  "E:/Deckplanet Data/Alpha Clash/Playtest Cards/Upates & New cards";

const outputPath = directoryPath + "/optimized";

function main() {
  fs.mkdirSync(outputPath, { recursive: true });

  const files = fs.readdirSync(directoryPath);

  files.forEach(async (file) => {
    if (path.extname(file).toLowerCase() === ".png") {
      const inputPath = path.join(directoryPath, file);
      const outputFileName = file; // keep same name, different folder
      const outputFilePath = path.join(outputPath, outputFileName);

      try {
        await sharp(inputPath)
          // Example tweaks:
          // .resize({ width: 640 }) // uncomment to downscale width if desired
          .png({
            quality: 70, // 0-100, lower is smaller file, more compression
            compressionLevel: 9, // 0-9, higher is smaller but a bit slower
            palette: true, // enable palette-based PNG for better compression
          })
          .toFile(outputFilePath);

        console.log(`Optimized: ${file}`);
      } catch (err) {
        console.error(`Error optimizing ${file}:`, err);
      }
    }
  });
}

main();
