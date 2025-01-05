import fs from "fs";
import sharp from "sharp";
import path from "path";

const directoryPath = 'E:/Deckplanet Data/Fusion World/FB05';

const outputPath = directoryPath + '/cropped';

async function cropImage(){
    fs.mkdirSync(outputPath, {recursive:true});
    fs.readdirSync(directoryPath).forEach(async (file) => {
        if(path.extname(file).toLowerCase() === '.jpg'){
            const inputPath = path.join(directoryPath, file);
            const outputPat = path.join(outputPath, file.split('.')[0] + '.png');
            try {
                await sharp(inputPath)
                .extract({left: 47, top: 62, width: 887, height: 1239})
                .png({
                    quality: 80,
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