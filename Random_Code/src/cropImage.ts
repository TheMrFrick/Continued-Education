import fs from "fs";
import sharp from "sharp";
import path from "path";

const directoryPath =
  "E:/Deckplanet Data/Alpha Clash TCG Digital Assets/Alpha Clash TCG Digital Assets/DeckPlanet/Need Cropping";


// const imageName = 'AC1-001.png';

const outputPath = directoryPath + '/cropped';

async function cropImage(){
    fs.mkdirSync(outputPath, {recursive:true});
    fs.readdirSync(directoryPath).forEach(async (file) => {
        if(path.extname(file).toLowerCase() === '.png'){
            const inputPath = path.join(directoryPath, file);
            const outputPat = path.join(outputPath, file.split('.')[0] + '.webp');
            try {
                await sharp(inputPath)
                .extract({left: 84, top: 96, width: 1460, height: 2030})
                .webp({
                    quality: 80,
                    lossless: false,
                    alphaQuality: 100,
                    effort: 6
                })
                .toFile(outputPat);

                console.log(`Processed: ${file}`);
            } catch (err) {
                console.error(`Error processing ${file}:`, err);
            }
        }
    })
}

cropImage();