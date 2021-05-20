# Courses API 

This repository contains the _backend_ of MyCourses application. It is a NodeJS/Express application with MongoDB persistence. It provides the following endpoints:

* GET `/api/courses`
* GET `/api/courses?status=status_value`
* GET `/api/courses/:courseId`
* POST `/api/courses`
* DELETE `/api/courses/:courseId`
* PATCH `/api/courses/:courseId`
* GET `/api/search`
* GET `/api/search?query=search_term&page=page_value&limit=limit_value`

MyCourses will be an application that allows a user (student) to search the CS courses catalogue (similar to SIS API) and organize courses into three categories:

* **Taken**: a course user has already taken (in the past)
* **Enrolled**: a course user is currently taking (in the present)
* **Interested**: a course user would like to take later (in the future) 

It is available at [this](https://hw3-courseapi.herokuapp.com) link
