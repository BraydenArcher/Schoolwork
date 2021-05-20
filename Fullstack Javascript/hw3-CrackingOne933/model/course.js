const mongoose = require("mongoose");
const Schema = mongoose.Schema;

const courseSchema = new Schema ({
    title:  {type: String, required: true}, //Add required attributes
    number: {type: String, required: true},
    term:   {type: String, required: true},
    status: {type: String, required: true, enum: ['interested', 'taken', 'enrolled']}, //Make sure status is only one of three
    __v:    {type: Number, select: false}, //Make sure __v is not returned on fetch
});
const Course = mongoose.model("Course", courseSchema); //Create new object with schema

module.exports = Course;