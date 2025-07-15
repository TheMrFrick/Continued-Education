import fs from "fs";
import sharp from "sharp";
import path from "path";

const directoryPath =
  "E:/Deckplanet Data/Alpha Clash/Playtest Cards/AC6 Test Cards";

const outputPath = directoryPath + "/output";

function main() {
  fs.mkdirSync(outputPath, { recursive: true });

  fs.readdirSync(directoryPath).forEach(async (file) => {
    if (path.extname(file).toLowerCase() === ".webp") {
      const inputPath = path.join(directoryPath, file);
      // const outputPat = path.join(outputPath, file); // to .png
      const outputPat = path.join(outputPath, file.split('.')[0] + '.png'); //to .webp
      try {
        await sharp(inputPath)
        //   .resize({ width: 640, height: 894 }) // Adjust width as needed
          .png({ quality: 50 })
          .toFile(outputPat);
        console.log(`Processed: ${file}`);
      } catch (err) {
        console.error(`Error processing ${file}:`, err);
      }
    }
  });
}

main();
