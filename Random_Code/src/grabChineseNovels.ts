import axios from "axios";
import * as cheerio from "cheerio";
import { writeFileSync } from 'fs';

const AxiosInstance = axios.create();

const url = "https://brokenjinsei.com/tyrannical-wangs-beloved-wife/";

async function main(){
    // grab links 
    const links = await grabChapterLinks();
    const pageData = await grabPageInfo(links);
    console.log(pageData.length);

    writeFileSync('./Tyrannical-Wangs-Beloved-Wife.txt',pageData.join('\n'));
}

async function grabChapterLinks(){
    const response = await AxiosInstance.get(url);
    // console.log(response);
    const body = await response.data;
    const $ = cheerio.load(body);

    return $(".arconix-accordion-content a").toArray().map((el) => {
        const elem = $(el);
        return el.attribs?.href
    })
}

async function grabPageInfo(links: string[]): Promise<any[]> {
    const pageData: string[][] = [];
    for(const link of links){
        try {
            const response = await AxiosInstance.get(link);
            const body = await response.data;
            const $ = cheerio.load(body);

            const textInfo = $(".entry-content p").toArray().map((pEl) => {
                const elem = $(pEl);
                return elem.text();
            })
            // console.log(textInfo);
            // break;
            pageData.push(textInfo);
        } catch (err) {
            console.log(err);
        }
    }
    return pageData;
}


main();