function whosPaying(names){
    const randNum = Math.round(Math.random() * names.length);
    return `${names[randNum]} is going to buy lunch today!`;
}

console.log(whosPaying(['Angela','Ben','Jenny',"Michael", 'Chloe']));