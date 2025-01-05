import { readdirSync, renameSync } from "fs";

const directoryPath =
  "E:/Deckplanet Data/Alpha Clash TCG Digital Assets/Alpha Clash TCG Digital Assets/Set 4 Genesis Returns/TC PROMO/output";

const possibleCardNums: Set<string> = new Set<string>();

async function main() {
  const dirItems = readdirSync(directoryPath);
  // console.log(dirItems);
  dirItems.forEach((v) => {
    // const parts = v.split(" ");
    // const filt = parts.filter((vp) => vp.includes("-"));
    // const num = filt[0].split('.')[0];
    // we may also have the num.png
    const parts = v.split("-");
    const num = parts[0] + '-' + parts[1];
    if (possibleCardNums.has(num)) {
      let i = 1;
      let mutated_name = num + "_AA";
      while (true) {
        if (!possibleCardNums.has(mutated_name)) {
          renameSync(
            directoryPath + "/" + v,
            // directoryPath + "/" + mutated_name +'.png'
            directoryPath + "/" + mutated_name +'.webp'
          );
          possibleCardNums.add(mutated_name);
          break;
        }
        if (i > 1) {
          mutated_name = num + "_AA" + i;
        }
        i++;
      }
    } else {
      // renameSync(directoryPath + "/" + v, directoryPath + "/" + num +'.png');
      renameSync(directoryPath + "/" + v, directoryPath + "/" + num +'.webp');
      possibleCardNums.add(num);
    }
    // possibleCardNums.push(filt[0]);
  });
  // console.log(hasDuplicates(possibleCardNums));
  // console.log(dirItems.length, possibleCardNums.size);
}

function hasDuplicates<T>(arr: T[]): boolean {
  return new Set(arr).size !== arr.length;
}

main();
