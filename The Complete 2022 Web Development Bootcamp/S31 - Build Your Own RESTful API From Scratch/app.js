const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const ejs = require('ejs');

// Set up application
const app = express();
app.set('view engine', 'ejs');
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static("public"));

// Mongoose Database information && setup
mongoose.connect("mongodb://localhost:27017/wikiDB");

const articleSchema = {
    title: String,
    content: String,
};

const Article = mongoose.model("article", articleSchema);

/**
 * RESTful route for '/articles'
 */
app.route('/articles')
    .get(async (req, res) => {
        const articles = await Article.find({});
        res.send(articles);
    })
    .post((req, res) => {
        const title = req.body.title;
        const content = req.body.content;
        const newArticle = new Article({
            title,
            content
        });
        newArticle.save((err) => {
            if (!err) {
                res.send("Successfully added a new article");
            } else {
                res.send(err);
            }
        });
    })
    .delete(async (req, res) => {
        Article.deleteMany((err) => {
            if (!err) {
                res.send("Successfully deleted all articles.");
            } else {
                res.send(err);
            }
        });
    });

/**
 * RESTful route for '/articles/:articleTitle'
 */
app.route("/articles/:articleTitle")
    .get(async (req, res) => {
        const article = await Article.findOne({title: req.params.articleTitle});
        if(article !== null){
            res.send(article);
        } else {
            res.sendStatus(404);
        }
    })
    .put(async (req, res) => {
        const result = await Article.updateOne({title: req.params.articleTitle}, {title: req.body.title, content: req.body.content})
        if(result.acknowledged){
            res.send("Successfully updated");
        } else {
            res.send("Error");
        }
    })
    .patch(async (req, res) => {
        const result = await Article.updateOne({title: req.params.articleTitle}, {$set: req.body});
        if(result.acknowledged){
            res.send("Successfully updated");
        } else {
            res.sendStatus(404);
        }
    })
    .delete(async (req, res) => {
        const result = await Article.deleteOne({title: req.params.articleTitle});
        if(result.acknowledged){
            res.send(`Deleted ${result.deletedCount} articles`);
        } else {
            res.send("Error in deleting");
        }
    });

/**
 * App Listener / Runner
 */
app.listen(3000, () => {
    console.log("App is listening on http://localhost:3000");
});