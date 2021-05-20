import axios from "axios";
require("dotenv").config(); //Env file reader
const JHED = process.env.REACT_APP_JHED;
const BASE_URL = process.env.REACT_APP_BASE_URL;

const headers = {
    Authorization: JHED,
};

async function getResponse(query, page, pageSize) { //get response with query page and size inputs
    try {
        return await axios.get(`${BASE_URL}/api/search?query=${query}&page=${page}&limit=${pageSize}`, { headers });
    } catch (err) {
        return [];
    }
}

async function getResponseWithLink(link) { //get response by link to make it easier to send
    try {
        return await axios.get(link, { headers });
    } catch (err) {
        return [];
    }
}

export { getResponse, getResponseWithLink }; //export those things