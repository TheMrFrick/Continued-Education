const express = require('express');
const bodyParser = require('body-parser');

const app = express();
app.use(bodyParser.urlencoded({ extended: true }));

app.get("/", (req, res) => {
    res.sendFile(__dirname + '/index.html');
})

app.post("/", (req, res) => {
    const num1 = Number(req.body.num1);
    const num2 = Number(req.body.num2);
    res.send(`Result of the calculation is ${num1 + num2}`);
});

app.get("/bmicalculator", (req, res) => {
    res.sendFile(`${__dirname}/bmiCalculator.html`);
})

app.post("/bmicalculator", (req, res) => {
    const weight = Number(req.body.weight);
    const height = Number(req.body.height);
    res.send(`Your BMI is ${weight / Math.pow(height,2)}`);
})

app.listen(3000, () => {
    console.log("calculator listening on port 3000");
})