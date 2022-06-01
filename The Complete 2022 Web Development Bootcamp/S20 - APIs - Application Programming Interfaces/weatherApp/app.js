const express = require('express');
const https = require('https');
const bodyParser = require('body-parser');

import gitSecrets from '../.gitSecrets.json';

const app = express();
app.use(bodyParser.urlencoded({ extended: true }));
const apiKey = gitSecrets.weatherAPI;
const units = 'imperial';
let query = 'Tewksbury';


app.get("/", (req, res) => {
    res.sendFile(`${__dirname}/index.html`);
});

app.post("/", (req, res) => {
    query = req.body.city || query;
    console.log(req.body);
    console.log(query);
    const url = `https://api.openweathermap.org/data/2.5/weather?appid=${apiKey}&q=${query}&units=${units}`;
    https.get(url, (response) => {
        if (response.statusCode === 200) {
            response.on('data', (data) => {
                const weatherData = JSON.parse(data);
                const temp = weatherData.main.temp;
                const weatherDescription = weatherData.weather[0].description;
                const icon = weatherData.weather[0].icon;
                const imageURL = `https://openweathermap.org/img/wn/${icon}@2x.png`;
                res.write(`<h1>The Temperature in ${weatherData.name} is ${temp} degrees Fahrenheit</h1>`);
                res.write(`<p>The weather is currently ${weatherDescription} </p>`);
                res.write(`<img src="${imageURL}"/>`);
                res.send();
            });
        } else {
            res.send("Error");
        }
    });
});

app.listen(3000, () => {
    console.log('server running on port 3000');
});