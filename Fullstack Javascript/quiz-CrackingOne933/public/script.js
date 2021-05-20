confirmedText = document.getElementById("confirmed");
activeText = document.getElementById("active");
recoveredText = document.getElementById("recovered");
dropDownList = document.getElementById("countries");

fetch('https://restcountries.eu/rest/v2/all') //Get data from rest countries
    .then(response => response.json()) //Feed response data into response.json
    .then(data => { //Feed data into this method to add to list
        data.slice().reverse().forEach(obj => { //Go through list of countries in object backwards, makes sure countries are alphabetical
            var newCountry = document.createElement('option');
            newCountry.text = obj.name; //Make sure display name is the name of the country
            newCountry.value = obj.alpha2Code; //And that the value is the code needed for the flag source
            if(newCountry.text > "United States of America") { //If it is after US just append to end
                dropDownList.append(newCountry);
            } else if (newCountry.text < "United States of America") { //If the country is alphabetically before US pre-append
                dropDownList.prepend(newCountry);
            } //US is not re added because of else if
        });
    });

fetch('https://www.trackcorona.live/api/countries/' + dropDownList.value)
    //This is the same as the method down below but specifically for the default country
    .then(response => response.json())
    .then(data =>{
        let confirmed;
        let active;
        let recovered;
        if(data.data.length !== 1) { //Just calculates all US data over
            confirmed = 'N/A';
            active = 'N/A';
            recovered = 'N/A';
        } else {
            confirmed = data.data[0].confirmed;
            recovered = data.data[0].recovered;
            let dead = data.data[0].dead;
            active = confirmed - (recovered + dead);
        }
        console.log();
        confirmedText.innerHTML = "<img src=\"https://www.countryflags.io/" + dropDownList.value + "/flat/24.png\" />"+ numberWithCommas(confirmed); //Auto updates US data
        activeText.innerHTML = "<img src=\"https://www.countryflags.io/" + dropDownList.value + "/flat/24.png\" />" + numberWithCommas(active); //When app starts
        recoveredText.innerHTML = "<img src=\"https://www.countryflags.io/" + dropDownList.value + "/flat/24.png\" />" + numberWithCommas(recovered);
    });



dropDownList.addEventListener("change", item => {
    let countryNumbers = '';
    fetch('https://www.trackcorona.live/api/countries/' + dropDownList.value) //Fetch data for current country
        .then(response => response.json())
        .then(data =>{
            let confirmed; //Establish variables for numbers and stuff
            let active;
            let recovered;
            if(data.data.length !== 1) { //If the country has no data than say N/A
                confirmed = 'N/A';
                active = 'N/A';
                recovered = 'N/A';
            } else { //Otherwise calculate correct numbers
                confirmed = data.data[0].confirmed;
                recovered = data.data[0].recovered;
                let dead = data.data[0].dead;
                active = confirmed - (recovered + dead);
            }
            console.log();
            confirmedText.innerHTML = "<img src=\"https://www.countryflags.io/" + dropDownList.value + "/flat/24.png\" />"+ numberWithCommas(confirmed); //Update text in display with flag and cases
            activeText.innerHTML = "<img src=\"https://www.countryflags.io/" + dropDownList.value + "/flat/24.png\" />" + numberWithCommas(active);
            recoveredText.innerHTML = "<img src=\"https://www.countryflags.io/" + dropDownList.value + "/flat/24.png\" />" + numberWithCommas(recovered);
        });
});

function numberWithCommas(x) {
    return x.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
}


