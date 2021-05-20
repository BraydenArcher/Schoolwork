//Server.js just listens for website stuff


const debug = require("debug")("bloo-chat");
let express = require('express');
let app = express();
const http = require("http").Server(app);

const port = process.env.PORT || 7000;

app.use(express.static('public'));

http.listen(port, () => {
    console.log(`Express app listening at http://localhost:${port}`); //Basic info about where the app is listening
});