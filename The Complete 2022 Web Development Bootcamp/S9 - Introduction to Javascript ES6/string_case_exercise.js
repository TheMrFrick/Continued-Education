
function main() {

    // 1. Create a var that stores the name that user enters via prompt.

    let name = prompt('What is your name?');

    // 2. Capitalize the first letter of their name;

    name = name.slice(0, 1).toUpperCase() + name.slice(1).toLowerCase();

    // 3. We use the capitalized version of their name to greet them using an alert

    alert("hello there " + name);

}

main();