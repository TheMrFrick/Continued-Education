require('dotenv').config();
const express = require('express');
const bodyParser = require('body-parser');
const ejs = require('ejs');
const mongoose = require('mongoose');
const md5 = require('md5');

const app = express();

app.use(express.static('public'));
app.set('view engine', 'ejs');
app.use(bodyParser.urlencoded({ extended: true }));

mongoose.connect("mongodb://localhost:27017/userDB");

const userSchema = new mongoose.Schema({
    email: String,
    password: String,
});

const User = new mongoose.model("User", userSchema);

app.route("/")
    .get((req, res) => {
        res.render("home");
    });

app.route('/login')
    .get((req, res) => {
        res.render("login");
    })
    .post((req, res) => {
        User.findOne({ email: req.body.username }, (err, foundUser) => {
            if (err) {
                console.log(err);
            } else {
                if (foundUser) {
                    if (foundUser.password === md5(req.body.password)) {
                        res.render("secrets");
                    }
                }
            }
        });
    });

app.route("/register")
    .get((req, res) => {
        res.render("register");
    })
    .post(async (req, res) => {
        const newUser = new User({
            email: req.body.username,
            password: md5(req.body.password),
        });
        newUser.save((err) => {
            if (err) {
                console.log(err);
            } else {
                res.render("secrets");
            }
        })
    });

app.listen(3000, () => {
    console.log("App is listening on http://localhost:3000");
});