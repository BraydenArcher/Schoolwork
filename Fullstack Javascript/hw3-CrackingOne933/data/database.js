require("dotenv").config(); //Env file reader
const dbname = 'courses-db';
const password = process.env.DB_ADMIN_PASSWORD || "password"; //Get DB password from environment
const URI = `mongodb+srv://courses-admin:${password}@courses-api.19yhw.mongodb.net/${dbname}?retryWrites=true&w=majority`;
const mongoose = require("mongoose");
const CourseDao = require("../model/CourseDao.js"); //Get course data object
const courses = new CourseDao(); //Create a courseDAO object
const option = { //Set up options
    useNewUrlParser: true,
    useUnifiedTopology: true,
    useFindAndModify: true,
};

function connect() {
    mongoose.connect(URI, option); //Connect to database

    mongoose.connection.on("open", () => { //On successful connect
        mongoose.connection.db.collection('courses').countDocuments(function(err, count) { //If database is empty add sample courses
            if(count === 0) {
                addSampleCourses();
            }
        });
        console.log("Successfully connected!!"); //Successful connect!
    });

    mongoose.connection.on("error", () => { //if error occurred notify user
        console.log("Error: Did you set the environment password?");
    });
}

function addSampleCourses() { //Add samples courses
    const samples = [ //Sample courses declaration
        {
            title: "Gateway Computing: Java",
            number: "500.112",
            term: "Fall 2019",
            status: "taken",
        },
        {
            title: "Intermediate Programming",
            number: "601.220",
            term: "Spring 2020",
            status: "taken",
        },
        {
            title: "Data Structures",
            number: "601.226",
            term: "Spring 2020",
            status: "taken",
        },
        {
            title: "Full-Stack JavaScript",
            number: "601.280",
            term: "Fall 2020",
            status: "enrolled",
        },
        {
            title: "Object-Oriented Software Engineering",
            number: "601.421",
            term: "Spring 2021",
            status: "interested",
        },
    ];
    for(let i = 0; i < samples.length; i++) { //Loop through and add to database
        courses.create(samples[i].title, samples[i].number, samples[i].term, samples[i].status).then();
    }
}

module.exports = { connect };