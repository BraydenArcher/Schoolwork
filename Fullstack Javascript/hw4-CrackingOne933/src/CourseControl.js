import React from "react";
import PropTypes from "prop-types";

function CourseControl(props) {
    const { course } = props; //get course from props
    return (<select value={course.status} onChange={ (e) => {
        props.onCourseUpdate(course, e.target.value); //Feed the new status to the course update function
    }}>
        <option value="move" disabled>
            Move to...
        </option>
        <option value="enrolled">
            Currently Enrolled
        </option>
        <option value="interested">Want to Take</option>
        <option value="taken">Already Took</option>
        <option value="none">None</option>
    </select>);
}

export default CourseControl;

CourseControl.propTypes = { //Prop stuff
    course: PropTypes.shape({
        _id: PropTypes.string,
        term: PropTypes.string,
        title: PropTypes.string,
        number: PropTypes.string,
        status: PropTypes.string
    }).isRequired,
    onCourseUpdate: PropTypes.func.isRequired,
};
