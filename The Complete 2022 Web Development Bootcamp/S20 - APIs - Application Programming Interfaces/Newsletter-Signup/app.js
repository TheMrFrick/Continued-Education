const express = require('express');
const bodyParser = require('body-parser');
const app = express();

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static('public'));

app.get("/", (req, res) => {
    res.sendFile(`${__dirname}/signup.html`);
});

app.post("/", (req, res) => {
    const { firstName, lastName, email } = req.body;
    console.log(firstName, lastName, email);
})

app.listen(3000, () => {
    console.log("Listening on http://localhost:3000");
})