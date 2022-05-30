function loveCalc(name1, name2){
    const randPercent = Math.floor((Math.random() * 100) + 1)
    console.log(`The chemistry between ${name1} and ${name2} is: ${randPercent}%`)
}

loveCalc('Kyle', 'Amy');