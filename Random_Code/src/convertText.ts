import clipboard from "clipboardy";
function convertText(str: string){
    let parts = str.split('\n');
    let newStr: string[] = ['Mana Vortex - '];
    for(let i = 0; i < parts.length; i++) {
        if(parts[i].length === 0){
            continue;
        }
        if(i === 0 || i === 1){
            let cardNum = getParenthesis(parts[i]);
            newStr.push(cardNum);
        } else {
            let cardNum = getParenthesis(parts[i]);
            newStr.push(parts[i][0] + ' ' + cardNum);
        }
    }

    console.log(newStr.join('\n'));
    clipboard.writeSync(newStr.join('\n'));
}

function getParenthesis(str: string){
    let rightParen = str.indexOf('(');
    let leftParen = str.indexOf(')');

    return str.substring(rightParen + 1, leftParen);
}


let str = `
1 Zamasu : Fused (FB02-036)

4 Pilaf (FB01-056)
4 Zamasu (FB01-042)
4 Trunks : Youth (FS02-08)
4 Son Goten (FS02-06)
4 Vegito (FB02-061)
4 Zamasu : Fused (FB02-044)
2 Gotenks (FS02-03)
3 Vegito (FB02-139)
2 Vegeta (FS02-13)
4 Zen-Oh (FB02-049)
1 Goku Black (FB01-039)
4 Videl (FS02-10)

4 Galick Gun (FS02-15)
3 Absolute Lightning (FB02-068)
3 Sinister Sickle (FB01-068)
`
convertText(str)