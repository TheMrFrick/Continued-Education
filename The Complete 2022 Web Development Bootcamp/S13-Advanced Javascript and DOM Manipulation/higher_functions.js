function calculator(num1, num2, operator) {
    return operator(Number(num1), Number(num2));
}

function add(num1, num2) {
    return num1 + num2;
}

function multiply(num1, num2) {
    return num1 * num2;
}

function subtract(num1, num2) {
    return num1 - num2;
}

function divide(num1, num2) {
    return num1 / num2;
}


console.log('Addition\t', calculator(process.argv[2], process.argv[3], add))
console.log('Subtraction\t', calculator(process.argv[2], process.argv[3], subtract))
console.log('Multiplication\t', calculator(process.argv[2], process.argv[3], multiply))
console.log('Division\t', calculator(process.argv[2], process.argv[3], divide))