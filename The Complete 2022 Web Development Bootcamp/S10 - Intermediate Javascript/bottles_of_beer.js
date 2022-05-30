function bottlesOfBeer(){
    for(let i = 99; i > 0; i--){
        console.log(`${i} bottles of beer on the wall\n${i} bottles of beer\ntake one down, pass it around, ${i-1} bottles of beer on the wall.`);
    }
    console.log('No more bottles of beer on the wall');
}

bottlesOfBeer();