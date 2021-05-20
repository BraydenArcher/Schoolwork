import axios from "axios";
require("dotenv").config(); //Env file reader
const JHED = process.env.REACT_APP_JHED; //Get env stuff for requests
const BASE_URL = process.env.REACT_APP_BASE_URL;

const headers = { //header declaration
    Authorization: JHED,
};

async function getAll() { //Basic request for getting all the courses
    try {
        const response = await axios.get(`${BASE_URL}/api/courses`, { headers }); //Send get request with info
        return response.data.data; //return data
    } catch (err) {
        return [];
    }
}

async function remove(id) { //Remove a course by id
    try {
        const response = await axios.delete(`${BASE_URL}/api/courses/${id}`, { headers }); //Send delete request with info
        return response.data.data; //return data
    } catch (err) {
        return null;
    }
}

async function add(course) {
    try {
        const response = await axios.post(`${BASE_URL}/api/courses`, //set post request with info
                course,
            { headers });
        return response.data.data; //return data
    } catch (err) {
        return null;
    }
}

async function updateStatus(id, status) {
    const data = { //Create data out of status
        status: status
    };
    try {
        const response = await axios.patch(`${BASE_URL}/api/courses/${id}`, //Send patch request with info
            data,
            { headers });
        return response.data.data; //return data
    } catch (err) {
        return null;
    }
}

export { getAll, remove, add, updateStatus };