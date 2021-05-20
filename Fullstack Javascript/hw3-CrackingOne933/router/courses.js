const express = require("express");
const router = express.Router();

const CourseDao = require("../model/CourseDao.js"); //Get course data object
const courses = new CourseDao(); //Create a courseDAO object

//Basic API handling
router.get("/api/courses", (req ,res) => { //Handle get requests
    const status = req.query.status; //Get the status
    courses
        .readAll(status)//If theres a status feed it to read all, read all determines if its undefined or not
        .then((course) => res.json({ data: course }))
        .catch((err) => errorHandler(res, 500, err)); //Catch errors
});

router.get("/api/courses/:id", (req, res) => { //Handle get with course ID
    const id = req.params.id;
    courses
        .read(id)//Read courses with given ID, returns course
        .then((course) => course ? res.json({ data: course }): errorHandler(res, 404, "Resource not found"))  //return course given or throw error if not found
        .catch((err) => errorHandler(res, 500, err)); //Catch errors
});

router.post("/api/courses", (req, res) => {
    const title = req.body.title; //Get stuff to look up course
    const number = req.body.number;
    const term = req.body.term;
    const status = req.body.status;

    courses
        .create(title, number, term, status)//Create course with attributes, if one missing mongoose throws error and the catch catches it
        .then((course) => res.status(201).json({ data: course })) //If no error then add to database
        .catch((err) => errorHandler(res, 400, err)); //Catch errors
});

router.delete("/api/courses/:id", (req, res) => {
    const id = req.params.id;  //Get id
    courses
        .delete(id)//Delete course with given ID
        .then((course) =>
            course
                ? res.json({ data: course })
                : errorHandler(res, 404, "Resource not found") //Return error 404 if course does not exist
        )
        .catch((err) => {
            errorHandler(res, 400, err); //Catch errors
        });
});

router.patch("/api/courses/:id", (req, res) => {
    const id = req.params.id; //Get id
    const status = req.body.status; //Get new status

    courses
        .update(id, status) //Update the course with the given status and id
        .then((course) =>
            course
                ? res.json({ data: course })
                : errorHandler(res, 404, "Resource not found") //If course ID is invalid say not found
        )
        .catch((err) => errorHandler(res, 400, err)); //Catch errors
});

function errorHandler(res, status, error) { //Error handler method
    res.status(status).json({
        errors: [
            {
                status: status,
                detail: error.message || error,
            },
        ],
    });
}

module.exports = router;