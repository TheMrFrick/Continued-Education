import cors from "cors";
import express, { Express } from "express";
import mongoose from "mongoose";
import todoRoutes from './routes';
import bodyParser from 'body-parser';

const app: Express = express();

const PORT: string | number = process.env.PORT || 4000;

app.use(bodyParser.json());
app.use(cors({}));
app.use(todoRoutes);

const uri: string = `mongodb://localhost:27017/${process.env.MONGO_DB}`

mongoose.connect(uri).then(() => app.listen(PORT, () => console.log(`Server running on http://localhost:${PORT}`))).catch((error: any) => {throw error});