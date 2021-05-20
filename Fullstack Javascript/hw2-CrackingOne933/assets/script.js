document.addEventListener("DOMContentLoaded", (_event) => {
  // Connect to socket.io
  const socket = io(); // automatically tries to connect on same port app was served from
  const username = document.getElementById("uname").innerText;
  const form = document.getElementById("chatForm");
  const messages = document.getElementById("messages");
  const messageToSend = document.getElementById("txt");
  form.addEventListener("submit", (event) => {
    socket.emit("message", {
      user: username,
      message: messageToSend.value,
    });
    messageToSend.value = "";
    event.preventDefault();
  });

  socket.on("connect", () => { //Trigger welcome message on user first connecting
    socket.emit("welcome", { //Trigger welcome event
      user: username, //Send username
      message: "Welcome "+ username + "!", //Send message
    });
  });

  socket.on("getUsername", () => { //Response to the server requesting the username of the client
    socket.emit("returnUsername", {user: username}); //Returns the username to server
  });

  // append the chat text message
  socket.on("message", (msg) => { //Client response to a message being sent
    const message = document.createElement("li"); //Create a new message
    message.innerHTML = `<strong>${msg.user}</strong>: ${msg.message}`; //Build the new message
    messages.appendChild(message);//Append message to text box
    messages.scrollTop = messages.scrollHeight; //Verify scroll isn't off screen
  });

  socket.on("join", (msg) => { //
      const message = document.createElement("li"); //Create a new message
      message.innerHTML = `<strong style="color: #008000">BlooChatApp</strong>: <text style="color:#008000"> ${msg.user} has joined the room</text>`; //Build the new message
      messages.appendChild(message);//Append message to text box
      messages.scrollTop = messages.scrollHeight; //Verify scroll isn't off screen
  });

  socket.on("onlineUsers", (msg) => {
    const message = document.createElement("li"); //Create a new message
    message.innerHTML = `<strong style="color: #008000">BlooChatApp</strong>: <text style="color:#008000"> ${msg.message}</text>`; //Build the new message
    messages.appendChild(message);//Append message to text box
    messages.scrollTop = messages.scrollHeight; //Verify scroll isn't off screen
  });

  socket.on("welcome", (msg) => {
    const message = document.createElement("li"); //Create a new message
    message.innerHTML = `<strong style="color: #008000">BlooChatApp</strong>: <text style="color:#008000">Welcome ${msg.user}!</text>`; //Build the new message
    messages.appendChild(message);//Append message to text box
    messages.scrollTop = messages.scrollHeight; //Verify scroll isn't off screen
  });

  socket.on("leave", (msg) => {
    const message = document.createElement("li"); //Create a new message
    message.innerHTML = `<strong style="color:#ff0000">${msg.user}</strong>: <text style="color:#ff0000">${msg.message}</text>`; //Build the new message
    messages.appendChild(message); //Append message to text box
    messages.scrollTop = messages.scrollHeight; //Verify scroll isn't off screen
  });

});
