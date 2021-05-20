import React, {Component} from "react";
import PropTypes from "prop-types";
import CourseCard from "./CourseCard";

//Map of statuses and what they should say on the header
const statusMap = new Map([["enrolled", "Currently Enrolled"], ["taken", "Already Took"], ["interested", "Want to Take"]]);

function CourseCategory(props) {
    const { status } = props;
    const courses = props.courses.filter(
        (course) => {
            return course.status === status;
        }
    );
    return (<div className="dashboard">
        <h2 className="dashboard-title">{statusMap.get(status)}</h2>
        <div className="dashboard-courses">
            <ol className="courses-grid">
                {
                    courses.map(
                        (course, index) => { //Map out each course to a card
                            return (<li key={index}>
                                <CourseCard //Feed props
                                    course={course}
                                    props={props}
                                    onCourseUpdate={props.onCourseUpdate}/>
                            </li>);
                        }
                    )
                }
            </ol>
        </div>
    </div>);
}

export default CourseCategory;

CourseCategory.propTypes = {
    courses: PropTypes.arrayOf (
        PropTypes.shape({
            _id: PropTypes.string,
            term: PropTypes.string,
            title: PropTypes.string,
            number: PropTypes.string,
            status: PropTypes.string
        })
    ).isRequired,
    onCourseUpdate: PropTypes.func.isRequired,
};
