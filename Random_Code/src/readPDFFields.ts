import fs from "fs";
import { PDFDocument } from "pdf-lib";

async function main() {
  const pdfBytes = fs.readFileSync("C:/Users/MrFri/OneDrive/Desktop/decklist_fw.pdf");
  const pdfDoc = await PDFDocument.load(pdfBytes);

  const form = pdfDoc.getForm();
  const fields = form.getFields();

  fields.forEach((field) => {
    console.log(field.getName(), field.constructor.name);
  });
}

main();
