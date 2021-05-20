# Bloo Chat

A simple realtime messaging application build with Node, Express, and Socket.io.

After cloning the application, run `npm install` to install the dependencies. 

To run the application, use the command `npm run dev`.

The application is deployed on [Heroku](https://bloochathomework2.herokuapp.com).

This app works by first establishing a socket connection with the client. Once a connection is 
established, the server gets the username from the client, and adds it to a list of online users, 
as well as displaying a welcome message to the client and displaying a joined message to other clients. 
When the connection to the client is lost, a left message is displayed to all clients using the username
 of the client. The users name is also removed from the list of online users, ensuring that it is 
 accurate. The server handles most of the logistics behind events while the client is actually what 
 executes the code behind those events. The style.css file just has some generic modifications to default tags
  in HTML. This is to ensure that the usernames are properly styled.