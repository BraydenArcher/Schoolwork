require("dotenv").config(); //Env file reader
let SISdata = [];
const key = process.env.SIS_KEY || "key"; //Get key from environment
const axios = require("axios");

async function cache() {
    console.log("fetching...");
    await axios.get(`https://sis.jhu.edu/api/classes?key=${key}&School=Whiting%20School%20of%20Engineering&Department=EN%20Computer%20Science`)
        .then(response => response.data)
        .then(data => {
            console.log("Beginning to parse SIS API data");
            for(let i = data.length - 1; i >= 0; i--) { //Go through array backwards so it's properly sorted
                let currentCourse = {
                    "title": data[i].Title,
                    "number": data[i].OfferingName.substring(3, 10),
                    "term": data[i].Term
                };
                if(!include(currentCourse)) { //If course isnt in collection already add it
                    SISdata.push(currentCourse);
                }
            }
            console.log("Got SIS api data");
        }).catch((err) => { //If error in SIS api, print it
            console.log(err.message);
        });
}
function returnData() { //return SISdata to outside object
    return SISdata;
}

function include(currentCourse) { //Check if title number and term all match another object in SISdata already
    for(let i = 0; i < SISdata.length; i++) {
        if(SISdata[i].title === currentCourse.title) {
            if(SISdata[i].number === currentCourse.number) {
                if(SISdata[i].term === currentCourse.term) {
                    return true;
                }
            }
        }
    }
    return false;
}

module.exports = {cache, returnData};
