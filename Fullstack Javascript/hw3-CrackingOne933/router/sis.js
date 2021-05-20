const express = require("express");
const router = express.Router();

const server = require("../index.js");
const SIS = require("../data/sis.js");
let host;

//Search CS courses
router.get("/api/search", (req ,res) => {
    host = req.get('host');
    const searchQuery = req.query.query ? req.query.query: ""; //get input parameters
    const page = req.query.page ? parseInt(req.query.page): 1;
    const limit =  req.query.limit ? parseInt(req.query.limit): 10;
    if(page < 1) { //Error handling for page
        errorHandler(res, 400, "Page must be >= 1");
    }
    if (limit < 1) { //Error handling for limit
        errorHandler(res, 400, "Limit must be >= 1");
    }
    generateSISResponse(res, searchQuery, page, limit); //If valid generate response
});

function getLinkObject(page, totalPages, searchQuery, limit) {
    if (page === totalPages && page !== 1) { //Create the appropriate object for links
        return {
            first: `http://${host}/api/search?query=${searchQuery}&page=1&limit=${limit}`, //If last page
            last: `http://${host}/api/search?query=${searchQuery}&page=${totalPages}&limit=${limit}`,
            prev: `http://${host}/api/search?query=${searchQuery}&page=${page - 1}&limit=${limit}`
        };
    } else if (page === 1) {
        return { //If first page
            first: `http://${host}/api/search?query=${searchQuery}&page=1&limit=${limit}`,
            last: `http://${host}/api/search?query=${searchQuery}&page=${totalPages}&limit=${limit}`,
            next: `http://${host}/api/search?query=${searchQuery}&page=${page + 1}&limit=${limit}`
        };
    } else { //If any other page
        return {
            first: `http://${host}/api/search?query=${searchQuery}&page=1&limit=${limit}`,
            last: `http://${host}/api/search?query=${searchQuery}&page=${totalPages}&limit=${limit}`,
            prev: `http://${host}/api/search?query=${searchQuery}&page=${page - 1}&limit=${limit}`,
            next: `http://${host}/api/search?query=${searchQuery}&page=${page + 1}&limit=${limit}`
        };
    }
}

function sendResponse(res, page, limit, totalPages, SISdata, nextLinks, searchQuery, length) {
    res.status(200).json({ //Generate response object
        pagination: { //Pagination
            page: page,
            limit: limit,
            last: totalPages,
            total: length
        },
        links: nextLinks, //Links object
        data: SISdata, //Data requested
        query: searchQuery //Query that was set
    });
}

//Function that returns true or false depending on if the data contains the query in number or title
function potentialMatch(data, query) {
    let title = data.title;
    let number = data.number;
    return title.toLowerCase().indexOf(query) !== -1 || number.indexOf(query) !== -1;
}

//Returns an object that only contains data with the selected query
function getFilteredResponse(SISdata, query) {
    if(query === "") { //If no query don't bother searching
        return SISdata;
    }
    let filteredData = [];
    for(let i = 0; i < SISdata.length; i++) {
        if(potentialMatch(SISdata[i], query)) { //If contains keywords
            filteredData.push(SISdata[i]); //Add to new data set
        }
    }
    return filteredData;
}


function generateSISResponse(res, searchQuery, page, limit) {
    const last = page * limit; //calculate the last entry on the page
    const SISdata = SIS.returnData(); //Get original SIS data
    let filteredData = getFilteredResponse(SISdata, searchQuery); //Get filtered data
    const length = filteredData.length; //Get total number of classes that match criteria
    let totalPages = Math.ceil(filteredData.length ? filteredData.length/limit: 1); //Calculate total number of pages based on limit and class number
    let nextLinks = getLinkObject(page, totalPages, searchQuery, limit); //Get object for link in response
    sendResponse(res, page, limit, totalPages, filteredData.slice(0 + page === 1 ? 0: (page-1) * limit, last), nextLinks, searchQuery, length); //Send response
}

function errorHandler(res, status, error) {
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