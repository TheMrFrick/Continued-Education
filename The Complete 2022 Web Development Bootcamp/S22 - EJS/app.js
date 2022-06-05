const express = require("express");
const bodyParser = require("body-parser");
const date = require(__dirname + "/date.js");

const app = express();
app.set('view engine', 'ejs');
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static("public"));

const todos = [];
const workItems = [];

app.get("/", (req, res) => {
    const day = date.getDate();
    res.render('list', { listTitle: day, todos, urlLink:"" });
});

app.post("/", (req, res) => {
    todos.push(req.body.newItem);
    res.redirect('/');
})

app.get("/work", (req, res) => {
    res.render("list", {listTitle: "Worklist", todos: workItems, urlLink: "work"})
})

app.post("/work", (req, res) => {
    workItems.push(req.body.newItem);
    res.redirect("/work");
})

app.get("/about", (req, res) => {
    res.render('about');
})

app.listen(3000, () => {
    console.log("Server started on http://localhost:3000");
});