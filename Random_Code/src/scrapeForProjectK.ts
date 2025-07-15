import axios from "axios";
import * as cheerio from "cheerio";
import { createWriteStream, writeFileSync } from "fs";

const AxiosInstance = axios.create();

const url =
  "https://spellmana.com/project-k-card-list-league-of-legends-trading-card-game/";

const imgUrls: string[] = [];

async function main() {
  try {
    const response = await AxiosInstance.get(url);
    const body = await response.data;
    const $ = cheerio.load(body);

    const imageEntrySection = $(".entry-content img")
      .toArray()
      .forEach((img) => {
        // console.log(img.attribs['data-src']);
        imgUrls.push(img.attribs["data-src"]);
      });
    // console.log(imageEntrySection);
    downloadImages();
  } catch (err) {
    console.log(err);
  }
}

async function downloadImages() {
  for (const imgLink of imgUrls) {
    const cardName = imgLink.substring(imgLink.lastIndexOf("/") + 1, imgLink.lastIndexOf("/") + 9 ).split('.')[0];
    let response = await AxiosInstance.get(imgLink, {
      responseType: "stream",
    });
    let fileName = `dataDownloads/${cardName}.webp`;
    response.data.pipe(createWriteStream(fileName));
    console.log("Image created for", cardName);
  }
}
main();
