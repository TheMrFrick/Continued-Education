// const { MongoClient } = require("mongodb");
// // Replace the uri string with your MongoDB deployment's connection string.
// const uri =
//   "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&writeConcern=majority";
// const client = new MongoClient(uri);
// async function run() {
//   try {
//     await client.connect();
//     const database = client.db('sample_mflix');
//     const movies = database.collection('movies');
//     // Query for a movie that has the title 'Back to the Future'
//     const query = { title: 'Back to the Future' };
//     const movie = await movies.findOne(query);
//     console.log(movie);
//   } finally {
//     // Ensures that the client will close when you finish/error
//     await client.close();
//   }
// }
// run().catch(console.dir);

const { MongoClient } = require("mongodb");
const assert = require("assert");

const url = "mongodb://localhost:27017";

const dbName = "fruitsDB";

const client = new MongoClient(url);

client.connect((err) => {
    assert.equal(null, err);
    console.log('Connected successfully to server');

    const db = client.db(dbName);
    findDocuments(db, () => {
        client.close();
    })
});

function insertDocuments(db, callback) {
    const collection = db.collection('fruits');

    collection.insertMany([
        {
            name: 'Apple',
            score: 8,
            review: 'Great fruit'
        },
        {
            name: "Orange",
            score: 6,
            review: 'Kinda sour'
        },
        {
            name: "Banana",
            score: 9,
            review: "Great Stuff!"
        }
    ], (err, result) => {
        assert.equal(err, null);
        assert.equal(3, result.result.n);
        assert.equal(3, result.ops.length);
        console.log("Inserted 3 documents into the collection");
        callback(result);
    });
}

function findDocuments (db, callback) {
    const collection = db.collection('fruits');
    collection.find({}).toArray((err, docs) => {
        assert.equal(err,null);
        console.log("Found the following records");
        console.log(docs);
        callback(docs);
    })
}