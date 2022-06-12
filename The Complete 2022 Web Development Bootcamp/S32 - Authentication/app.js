const express = require('express');
const bodyParser = require('body-parser');
const ejs = require('ejs');
const mongoose = require('mongoose');
const encrypt = require('mongoose-encryption');

const app = express();

app.use(express.static('public'));
app.set('view engine', 'ejs');
app.use(bodyParser.urlencoded({ extended: true }));

mongoose.connect("mongodb://localhost:27017/userDB");

const userSchema = new mongoose.Schema({
    email: String,
    password: String,
});

const secret = "Thisisourlittlesecret.";
userSchema.plugin(encrypt, { secret, encryptedFields: ['password'] });

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
                    if (foundUser.password === req.body.password) {
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
            password: req.body.password,
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