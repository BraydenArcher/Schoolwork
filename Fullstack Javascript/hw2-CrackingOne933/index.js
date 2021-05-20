const debug = require("debug")("bloo-chat");
const nunjucks = require("nunjucks");
const express = require("express");
const app = express();
const http = require("http").Server(app);
const io = require("socket.io")(http);

const port = process.env.PORT || 7000;

nunjucks.configure("views", {
  autoescape: true,
  express: app,
});

app.use(express.static("assets"));

app.get("/", (req, res) => {
  res.render("base.tmpl", null); //Get base template
});

app.get("/chatroom", (req, res) => {
  res.render("chatroom.njk", { uname: req.query.uname }); //Get chatroom that inherits the base template and modifies the body
});

let users = []; //Create list for users who are online


io.on("connection", function (socket) {
  let username; //Temporary username function to store username when user leaves
  socket.emit("getUsername"); //Tell client that I want their username
  socket.on("message", (msg) => { //If a client send a message to me
    debug(`${msg.user}: ${msg.message}`); //Sanitize user input
    io.emit("message", msg); //Broadcast message to everyone
  });
  socket.on("welcome", (msg) => { //If a user joins
    debug(`${msg.user}: ${msg.message}`); //Sanitize user input
    socket.emit("welcome", msg); //Show welcome message to only that user
    let online; //temporary variable for creating the message about users online
    if(users.length === 0) {
      online = "Unfortunately no one is online at the moment :(" //Display message for when nobody is online
    } else {
      online = "Online Users: " + users; //If someone is online, display their names
    }
    socket.emit("onlineUsers", { //Send the list of online users to the client that joined
      user: "BlooChatApp",
      message: online,
    });
    socket.broadcast.emit("join", msg); //Broadcast a join message to everybody except the client who joined
  });

  socket.on("returnUsername", (user) => { //Event for when client send their username
    username = user.user; //Receive username into username variable
    users.push(user.user); //Push that username into the online users list
  });
  socket.on("disconnect", () => { //If the user loses connection
    io.emit("leave", { //Trigger leave event in all clients with the username of the user who left
      user: "BlooChatApp",
      message: username + " has left the room",
    });
    for(let i = 0; i < users.length; i++) { //For loop to check online users
      if(users[i] === username) { //If the current element is the user who left,
        users.splice(i, 1); // remove from online users list
        break; //Break ensures that if two users have the same username, the loop only removes one of the two,thus ensuring the list is always accurate
      }
    }
  });
});

http.listen(port, () => {
  console.log(`Express app listening at http://localhost:${port}`); //Basic info about where the app is listening
});
