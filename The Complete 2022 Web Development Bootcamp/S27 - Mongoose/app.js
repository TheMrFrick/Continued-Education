const mongoose = require('mongoose');

// Connect to database
mongoose.connect("mongodb://localhost:27017/fruitsDB");

// Create a schema
const fruitSchema = new mongoose.Schema({
    name: {
        type: String,
        required: [true, 'Check your data entry, no name specified']
    },
    rating: {
        type: Number,
        min: 1,
        max: 10
    },
    review: String,
});

const Fruit = mongoose.model("Fruit", fruitSchema);

// const fruit = new Fruit({
    // name: "Apple",
//     rating: 7,
//     review: "Pretty solid as a fruit."
// });

// fruit.save();

const personSchema = new mongoose.Schema({
    name: String,
    age: Number,
});

const Person = mongoose.model("Person", personSchema);

const person = new Person ({
    name: "John",
    age: 37
});

person.save();

// const kiwi = new Fruit({
//     name: "Kiwi",
//     rating: 10,
//     review: "the best fruit!"
// });

// const orange = new Fruit(
// {
//     name: "Orange",
//     rating: 4,
//     review: 'too sour for me'
// }
// );

// const banana = new Fruit({
//     name: "banana",
//     rating: 3,
//     review: "weird texture"
// });

// Fruit.insertMany([kiwi, orange, banana]);


Fruit.find((err, fruits) => {
    fruits.forEach((fruit) => {
        console.log(fruit.name, fruit.rating);
    });

});


// Fruit.deleteOne({_id:'62a3362dccd5c6e28df1581e'}).then((res) => {
//     console.log(res.matchedCount);
// });

// Person.deleteMany({name: "John"}).then((res) => {
//     console.log(res.deletedCount);
// })
// function findDocuments (db, callback) {
//     const collection = db.collection('fruits');
//     collection.find({}).toArray((err, docs) => {
//         assert.equal(err,null);
//         console.log("Found the following records");
//         console.log(docs);
//         callback(docs);
//     })
// }