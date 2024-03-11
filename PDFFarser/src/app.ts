import { PdfPage, readPdfPages, readPdfText } from "pdf-text-reader";
import * as fs from "fs";

const PDF_URLS = [
  "https://www.dbs-cardgame.com/fw/pdf/rules/fw_faq_pr_en.pdf",
  "https://www.dbs-cardgame.com/fw/pdf/rules/fw_faq_fs01-04_en_240216.pdf",
  "https://www.dbs-cardgame.com/fw/pdf/rules/fw_faq_fb01_en_240216.pdf",
];
const fileName = "fw_qa";

interface IQNA {
  qas: innerQAS[];
  cardID: string;
  name: string;
}

interface innerQAS {
  question: string;
  answer: string;
}

const QNAs: IQNA[] = [];

async function readPDF(url: string) {
  return await readPdfPages({ url });
}

async function main() {
  for (const url of PDF_URLS) {
    const pdfPage: PdfPage[] = await readPDF(url);
    for (const page of pdfPage) {
      let inQuestions: boolean = false;
      let inAnswers: boolean = false;
      const pdfLines = page.lines;
      // const pdfLines = pdfText.split("\n");
      let qas: innerQAS[] = [];
      let cardID: string = "";
      let name: string = "";
      let currQuesInd = 0;
      for (let i = 0; i < pdfLines.length; i++) {
        const thisLine = pdfLines[i];
        console.log(thisLine);
        if (thisLine === "") {
          const nextLine = pdfLines[i + 1];
          if (inAnswers && !nextLine.startsWith("Q")) {
            QNAs.push({
              cardID,
              name,
              qas,
            });
            cardID = "";
            name = "";
            qas = [];
            inAnswers = false;
          }

          continue;
        }
        if (inQuestions) {
          if (thisLine.startsWith(`A${currQuesInd + 1}.`)) {
            inQuestions = false;
            inAnswers = true;
            qas[currQuesInd].answer = thisLine.trim().substring(4);
          } else {
            qas[currQuesInd].question += ` ${thisLine.trim()}`;
          }
        } else if (inAnswers) {
          if (thisLine.startsWith(`Q${currQuesInd + 2}.`)) {
            inQuestions = true;
            inAnswers = false;
            qas.push({ question: thisLine.trim().substring(4), answer: "" });
            currQuesInd++;
          } else {
            qas[currQuesInd].answer += ` ${thisLine.trim()}`;
          }
        } else if (
          thisLine.includes("-") &&
          (thisLine.startsWith("FS") ||
            thisLine.startsWith("FB") ||
            thisLine.startsWith("FP"))
        ) {
          cardID = thisLine.split(" ")[0];
          name = thisLine;
          inAnswers = false;
          inQuestions = false;
        } else if (thisLine.startsWith("Q1.")) {
          currQuesInd = 0;
          inQuestions = true;
          qas.push({ question: thisLine.trim().substring(4), answer: "" });
        }
      }
      console.log("");
      QNAs.push({
        cardID,
        name,
        qas,
      });
      cardID = "";
      name = "";
      qas = [];
      inAnswers = false;
      inQuestions = false;
    }
  }

  fs.writeFileSync(`${fileName}.json`, JSON.stringify(QNAs));
}

main();
