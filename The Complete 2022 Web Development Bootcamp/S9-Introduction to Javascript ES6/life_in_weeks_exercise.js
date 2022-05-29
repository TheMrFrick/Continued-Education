function lifeInWeeks(age) {
    const yearsLeft = 90 - age;
    console.log(`You have ${yearsLeft * 365} days, ${yearsLeft * 52} weeks, and ${yearsLeft * 12} months left.`)
}

lifeInWeeks(25);