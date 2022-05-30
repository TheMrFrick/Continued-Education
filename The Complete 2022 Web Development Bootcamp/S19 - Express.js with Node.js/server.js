const express = require('express');
const app = express();

app.get("/", (request, response) => {
    response.send("<H1>Hello, world!</h1>");
});

app.get('/contact', (req, res) => {
    res.send("Contact me at: themrfrick@gmail.com");
});

app.get('/about', (req, res) => {
    res.send('A brief bio about who owns this page.');
});

app.listen(3000, () => {
    console.log('Server is running at localhost:3000');
});