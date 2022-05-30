function bmiCalculator(weight, height){
    const bmi = weight / (Math.pow(height,2));
    if(bmi < 18.5){
        return `You're BMI is ${bmi}, so you are underweight.`;
    } else if(bmi > 24.9) {
        return `You're BMI is ${bmi}, so you are overweight.`;
    }
    return `You're BMI is ${bmi}, sor you have a normal weight.`;
}

console.log(bmiCalculator(80,2))