import React from "react";
import PropTypes from "prop-types";
import CourseControl from "./CourseControl";
import Color from "./Color";

function CourseCard(props) {
    //Just returns the color grids
    const { course } = props;
    return (<div className="course">
        <div className="course-top">
            <svg className="course-logo">
                <rect
                    x={0}
                    y={0}
                    width={128}
                    height={128}
                    fill={Color.getColor(course.title)}
                />
                <text
                    x="50%"
                    y="35%"
                    textAnchor="middle"
                    dominantBaseline="middle"
                    fill="white"
                    fontFamily="monospace"
                    fontSize="2.2em"
                >
                    <tspan x="50%" dy="0">
                        {course.number.split(".")[0]}
                    </tspan>
                    <tspan x="50%" dy="1.2em">
                        {course.number.split(".")[1]}
                    </tspan>
                </text>
            </svg>
            <div className="course-status-changer">
                <CourseControl course={course} props={props} onCourseUpdate={props.onCourseUpdate}/>
            </div>
        </div>
        <div className="course-title">
            {course.title}
        </div>
        <div className="course-term">{course.term}</div>
    </div>);
}

export default CourseCard;

CourseCard.propTypes = { //Just define props
    course: PropTypes.shape({
            _id: PropTypes.string,
            term: PropTypes.string,
            title: PropTypes.string,
            number: PropTypes.string,
            status: PropTypes.string
        }).isRequired,
    onCourseUpdate: PropTypes.func.isRequired,
};
