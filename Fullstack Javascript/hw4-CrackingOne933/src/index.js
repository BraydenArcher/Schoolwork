import ReactDOM from "react-dom";
import App from "./App";
import "./index.css";
import { BrowserRouter as Router } from "react-router-dom";

ReactDOM.render( //Render the App in a Router for react routers
    <Router>
        <App />
    </Router>,
    document.getElementById("root"));