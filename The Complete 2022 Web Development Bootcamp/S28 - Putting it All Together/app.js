const express = require("express");
const bodyParser = require("body-parser");
const mongoose = require('mongoose');

const app = express();
app.set('view engine', 'ejs');
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static("public"));

mongoose.connect("mongodb://localhost:27017/todolistDB");

const itemSchema = new mongoose.Schema({
    name: String,
});

const Item = new mongoose.model('item', itemSchema);

const listSchema  = {
    list: String,
    items: [itemSchema]
}

const List = new mongoose.model("List", listSchema);

// ONLY UNCOMMENT FOR FIRST RUN
const item1 = new Item({ name: "Welcome to you todo list" });
const item2 = new Item({ name: "Hit the + button to add a new item" });
const item3 = new Item({ name: "<-- hit this item to delete an item" });
const defaultItems = [item1, item2, item3];

app.get("/", async (req, res) => {
    const todos = await Item.find({});
    if (todos.length === 0) {
        await Item.insertMany(defaultItems);
        res.redirect('/');
    } else {
        res.render('list', { listTitle: 'Today', todos, urlLink: "" });
    }
});

app.post("/", (req, res) => {
    const newItem = new Item({
        name: req.body.newItem
    });
    newItem.save();
    res.redirect('/');
})

app.post("/:list", async (req, res) => {
    const listName = req.params.list.toLowerCase();
    let list = await List.findOne({list: listName});
    const newItem = new Item({
        name: req.body.newItem
    });
    list.items.push(newItem);
    await list.save();
    res.redirect(`/${listName}`);
})

app.post("/delete/:id", async (req, res) => {
    const listName = req.body.listName;
    if(listName !== "Today"){
        await List.findOneAndUpdate({name: listName}, {$pull: {items: {_id: req.params.id }}})
        res.redirect(`/${listName}`);
    } else {
        await Item.findByIdAndRemove(req.params.id);
        res.redirect('/');
    }

});

app.get("/:list", async (req, res) => {
    const listName = req.params.list.toLowerCase();
    console.log(listName);
    let list = await List.findOne({list: listName});
    console.log(list);
    if(list === undefined || list === null){
        console.log("didn't exist");
        list = new List({
            list: listName,
            items: defaultItems,
        });
        await list.save();
    }
    console.log(list.items);
    res.render('list', {listTitle: listName, todos: list.items, urlLink: listName})

})

app.get("/about", (req, res) => {
    res.render('about');
})

app.listen(3000, () => {
    console.log("Server started on http://localhost:3000");
});