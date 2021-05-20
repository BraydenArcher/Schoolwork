import PropTypes from "prop-types";
import React from "react";
const leftArrow = "<";

function Pagination(props) {
    if (props.pagination === undefined) { //If the pagination is undefined return nothing
        return null;
    }
    const currentPage = props.pagination.page;
    const lastPage =  props.pagination.last;
    if (lastPage === 1 || lastPage === 0) { //If there is no need return nothing
        return null;
    }

    if (lastPage === 2 && currentPage === 2) { //If you are on page 2/2 custom render two buttons
        return (
            <React.Fragment>
                <button onClick={props.firstPage}> 1 </button>
                <button disabled> 2 </button>
            </React.Fragment>);
    } else if (lastPage === 2 && currentPage === 1) {
        return (
            <React.Fragment>
                <button disabled > 1 </button>
                <button onClick={props.lastPage}> 2 </button>
            </React.Fragment>);
    }

    if(lastPage === currentPage) { //if on last page exclude next arrow
        return (
            <React.Fragment>
                <button onClick={props.firstPage}> 1 </button>
                <button onClick={props.previousPage}> {leftArrow} </button>
                <button disabled> {currentPage} </button>
            </React.Fragment>
        )
    } else if (currentPage === 1 && lastPage !== 1) { //If on page 1 dont include back button
        return (
            <React.Fragment>
                <button disabled> 1 </button>
                <button onClick={props.nextPage}> > </button>
                <button onClick={props.lastPage}> {lastPage} </button>
            </React.Fragment>
        )
    } else if (currentPage === 2) { //If on second page dont show back button but button 1 instead
        return (
            <React.Fragment>
                <button onClick={props.firstPage}> 1 </button>
                <button disabled> 2 </button>
                <button onClick={props.nextPage}> > </button>
                <button onClick={props.lastPage}> {lastPage} </button>
            </React.Fragment>
        )
    }
    return ( //If not any of the above cases display generic button layout
        <React.Fragment>
            <button onClick={props.firstPage}> 1 </button>
            <button onClick={props.previousPage}> {leftArrow} </button>
            <button disabled> {currentPage} </button>
            <button onClick={props.nextPage}> > </button>
            <button onClick={props.lastPage}> {lastPage} </button>
        </React.Fragment>);
}


export default Pagination;

Pagination.propTypes = { //Props stuff
    pagination: PropTypes.any,
    nextPage: PropTypes.func,
    previousPage: PropTypes.func.isRequired,
    firstPage: PropTypes.func.isRequired,
    lastPage: PropTypes.func.isRequired,
};