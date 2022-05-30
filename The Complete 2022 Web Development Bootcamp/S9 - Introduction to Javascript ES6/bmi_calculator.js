function bmiCalculator(weight, height){
    return Math.floor((weight) / Math.pow(height, 2));
}

console.log(bmiCalculator(65,1.8));