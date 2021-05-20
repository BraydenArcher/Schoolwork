import React, {Component} from "react";
import {Link} from "react-router-dom";
import PropTypes from "prop-types";
import "./SearchAPI"
import CourseCard from "./CourseCard";
import * as SearchAPI from "./SearchAPI";
import Pagination from "./Pagination";
import * as CoursesAPI from "./CoursesAPI";
import Color from "./Color";

class SearchPage extends Component {
    state = { //State for basic stuff that needs to be tracked
        coursesBeingDisplayed: [], //Courses currently on page
        coursesCache: [], //Courses that are in the database right now
        response: undefined, //Contains response object from last request
    }

    componentDidMount() { //On mount get all the courses and cache them to compare
        CoursesAPI.getAll().then((courses) => this.setState({ coursesCache: courses }));
    }

    updateQuery = () => { //If the query is updated
        const value = document.getElementById("searchBar").value; //get query
        if (value === "") { //If its empty show nothing
            this.setState({coursesBeingDisplayed: [], response: undefined}); //Update response and coursesBeingDisplayed
            return;
        }
        SearchAPI.getResponse(value, 1, 10) //otherwise get response with query and pages value
            .then(r => {
                //const newCourses = this.newCoursesList(r.data.data);
                const newValue = document.getElementById("searchBar").value; //get query again
                if (newValue === value) { //if the query is not the same don't update because we don't want dumb results!
                    this.setState({ response: r, coursesBeingDisplayed: r.data.data});
                }
            });
    }



    newCoursesList = (responseData) => { //Return a courses list with the correct statuses
        return responseData.map((course) => {
            course.status = "none";
            this.state.coursesCache.forEach((c) => { //Compare each course in the ones being displayed with the cached ones
                //and if the cached ones match take their status and give it to displayed ones
                if (c.title === course.title && c.term === course.term && c.number === course.number) {
                    course.status = c.status;
                }
            });
            return course;
        });
    }

    onCourseUpdate = (course, status) => { //If a course needs a status update
        if (course.status === "none") { //If the status is none add the course cause its not there yet
            course.status = status;
            this.addCourse(course);
            return;
        }
        let noSkip = true;
        this.state.coursesCache.forEach((c) => { //Check each course in cache to get equivalent with ID
            if (c.title === course.title && c.term === course.term && c.number === course.number && noSkip) {
                course = c;
                noSkip = false;
            }
        });

        const { _id } = course;
        if (status === "none") { //If new status is none then remove
            this.removeCourse(course);
            return;
        }
        CoursesAPI.updateStatus(_id, status).then(c => { //Update status with API
            if (c === null) {
                return;
            }
            this.props.removeCourse(c); //remove locally from homepage
            this.setState((state) => { //set state without the course
                return {
                    coursesCache: state.coursesCache.filter((c) => c._id !== course._id),
                };
            }, () => {
                course.status = status; //update status
                this.props.addCourse(c); //add course locally to homepage
                this.setState({ //set state with course again
                    coursesCache: this.state.coursesCache.concat(c)
                });
            });
        });

    }

    addCourse = (course) => { //Add course with API and locally
        CoursesAPI.add(course).then((c) => { //Add course to API
            if (c === null) {
                alert("Error adding Course");
                return;
            }
            this.setState(() => { //Update local courses list
                const temp = this.state.coursesCache;
                temp.push(c);
                return {courses: temp};
            }, ()=> {
                this.props.addCourse(c);
            });
        });
    };

    removeCourse = (course) => { //Remove the course from API and locally
        this.setState((state) => { //Remove locally
            return {
                coursesCache: state.coursesCache.filter((c) => c._id !== course._id),
            };
        });
        CoursesAPI.remove(course._id).then(r => { //Remove from API
            if (r === null) {
                return;
            }
            this.props.removeCourse(r); //Remove course from local homepage
        });
    };

    nextPage = () => { //get next page courses from link
        SearchAPI.getResponseWithLink(this.state.response.data.links.next)
            .then(r => {
                const newCourses = this.newCoursesList(r.data.data);
                this.setState({ response: r, coursesBeingDisplayed: newCourses});
            });
    }

    previousPage = () => { //get previous page courses from link
        SearchAPI.getResponseWithLink(this.state.response.data.links.prev)
            .then(r => {
                const newCourses = this.newCoursesList(r.data.data);
                this.setState({ response: r, coursesBeingDisplayed: newCourses});
            });
    }

    firstPage = () => { //get first page courses from link
        SearchAPI.getResponseWithLink(this.state.response.data.links.first)
            .then(r => {
                const newCourses = this.newCoursesList(r.data.data);
                this.setState({ response: r, coursesBeingDisplayed: newCourses});
            });
    }

    lastPage = () => { //get last page courses from link
        SearchAPI.getResponseWithLink(this.state.response.data.links.last)
            .then(r => {
                const newCourses = this.newCoursesList(r.data.data);
                this.setState({ response: r, coursesBeingDisplayed: newCourses});
            });
    }

    render() {
        const coursesBeforeUpdate = this.state.coursesBeingDisplayed; //Get courses list before status update
        const courses = this.newCoursesList(coursesBeforeUpdate); //get courses list with updated statuses

        const response = this.state.response === undefined ? undefined: this.state.response.data; //Get response data
        const pagination = response === undefined ? undefined: response.pagination; //get pagination data
        //Feed the data to the Course Cards below
        return (
            <div className="search-courses">
                <div className="search-courses-bar">
                    <Link to="/">
                        <button className="close-search" />
                    </Link>
                    <div className="search-courses-input-wrapper">
                        <input
                            id="searchBar"
                            type="text"
                            placeholder="Search by course title or course number"
                            onInput={this.updateQuery}
                        />
                    </div>
                </div>
                <div className="search-courses-results">
                    <ol className="courses-grid">
                        {
                            courses.map(
                                (course, index) => {
                                    return (<li key={index}>
                                        <CourseCard
                                            course={course}
                                            color={Color.getColor(course.title)}
                                            props={this.props}
                                            onCourseUpdate={this.onCourseUpdate}
                                        />
                                    </li>);
                                }
                            )
                        }
                    </ol>
                    <div className="search-courses-buttons">
                        <Pagination
                            pagination={pagination}
                            nextPage={this.nextPage}
                            previousPage={this.previousPage}
                            firstPage={this.firstPage}
                            lastPage={this.lastPage}
                        />
                    </div>
                </div>
            </div>
        );
    }
}

export default SearchPage;

SearchPage.propTypes = { //Prop stuff
    onCourseUpdate: PropTypes.func.isRequired,
    getCourses: PropTypes.func.isRequired,
    addCourse: PropTypes.func.isRequired,
    removeCourse: PropTypes.func.isRequired
};
