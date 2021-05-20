import React, { Component } from "react";
import { Route, Switch } from "react-router";
import "./App.css";
import * as CoursesAPI from "./CoursesAPI.js";
import SearchPage from "./Search";
import { Link } from "react-router-dom";
import CourseCategory from "./CourseCategory";

const statuses = ["enrolled", "interested", "taken"]
class CourseApp extends Component {
  state = {
    courses: [
    ],
  };

  onCourseUpdate = (course, status) => { //If a course's status is changed
    const { _id } = course; //Get the courses id
    if (status === "none") {  //If the status is none then
      this.removeCourse(course); //Remove the course from dashboard and API
      return; //and exit
    }
    CoursesAPI.updateStatus(_id, status).then(c => { //Update the status in the API
      if (c === null) { //If API failed just return
        return;
      }
      this.setState((state) => { //Otherwise add the returned course to the state
        return {
          courses: state.courses.filter((c) => c._id !== course._id), //by removing the course from before
        };
      }, () => {
        this.setState({
          courses: this.state.courses.concat(c) //and adding a new one
          // (because its easy and idk how to directly change the status)
        });
      });

    });
  }

  addCourse = (course) => { //Add a course
    CoursesAPI.add(course).then((c) => { //Use API to add it
      if (c === null) { //If it failed just return
        return;
      }
      this.setState({ //Set state with new received course locally
        courses: this.state.courses.concat(c)
      });
    });
  };

  addCourseLocally = (course) => { //A method for JUST adding the course locally, for outside use
    this.setState({
      courses: this.state.courses.concat(course)
    });
  }

  removeCourse = (course) => { //Remove a course
    this.setState((state) => { //Remove locally
      return {
        courses: state.courses.filter((c) => c._id !== course._id),
      };
    });
    CoursesAPI.remove(course._id).then(r => {}); //Then remove it from the API
  };

  removeCourseLocally = (course) => { //A method for JUST removing the course locally, for outside use
    this.setState((state) => {
      return {
        courses: state.courses.filter((c) => c._id !== course._id),
      };
    });
  }

  componentDidMount() { //On mount update courses with database
    CoursesAPI.getAll().then((courses) => this.setState({ courses }));
  }

  getCourses = () => { //Just returns the courses from here, for outside use
    return this.state.courses;
  }


  render() {
    const { courses }  = this.state;
    return (
      /**
       * TODO: Refactor this code to reusable React components
       */
      <div className="app">
        {
          <Switch>
            <Route exact path="/">
              <div className="list-courses">
                <div className="list-courses-title">
                  <h1>My Courses</h1>
                </div>
                <div className="list-courses-content">
                  <div>
                    {
                      statuses.map( //Map each status to a section
                          (status, index) => {
                            return (
                                <CourseCategory //Return a course category for each status
                                    courses={courses} //feed the courses
                                    status={status} //feed the status
                                    onCourseUpdate={this.onCourseUpdate} //Feed the update method
                                    key={index} //Feed index for logistic reasons
                                />);
                          }
                      )
                    }
                  </div>
                </div>
                <div className="open-search">
                  <Link to="/search">
                    <button/>
                  </Link>
                </div>
              </div>
            </Route>
            <Route exact path="/search">
                <SearchPage //Create a search page
                    onCourseUpdate={this.onCourseUpdate} //function to update status here
                    getCourses={this.getCourses} //function to get courses
                    addCourse={this.addCourseLocally} //function to add to local courses here
                    removeCourse={this.removeCourseLocally} //function to remove local courses here
                />
            </Route>
          </Switch>
        }
      </div>
    );
  }
}

export default CourseApp;
