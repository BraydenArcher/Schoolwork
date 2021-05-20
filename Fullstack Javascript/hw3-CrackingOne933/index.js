require("dotenv").config(); //Env file reader
const express = require("express");
const app = express();
const port = process.env.PORT || 4567; //Get port from environment
let host;

const courseRouter = require("./router/courses.js"); //Require statements
const sisRouter = require("./router/sis.js");
const database = require("./data/database.js");
const helmet = require("helmet");
const cors = require("cors");
const morgan = require("morgan");
const SISapi = require("./data/sis.js");

//App usage
app.use(helmet()); //Add middleware
app.use(cors());
app.use(morgan("dev"));
app.use(express.json()); //Add express.json()
app.use(courseRouter); //Add routers
app.use(sisRouter);

//Connect database
database.connect();

//Cache SIS api
SISapi.cache();

app.listen(port, () => {
    console.log(`Server is listening ${app.address ? app.address: "localhost"}:${port}`);
});