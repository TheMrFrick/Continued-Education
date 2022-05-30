function isLeap(year){
    if(year % 4 === 0){
        if(year % 100 === 0){
            if(year % 400 === 0){
                console.log("Leap year.")
                return true;
            }
            console.log("Not leap year.")
            return false;
        }
        console.log("Leap year.")
        return true;
    }
    console.log("Not leap year.")
    return false;
}