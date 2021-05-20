const Course = require("./course");

class CourseDao {
    constructor() {
    }

    async create(title, number, term, status) { //Create note based on criteria
        return await Course.create({
            title,
            number,
            term,
            status,
        });
    }

    async readAll(status = "") { //Read all notes with status given
        const filter = status ? {status } : {};
        return await Course.find(filter);
    }

    async read(id) { //Read notes with ID
        return await Course.findById(id);
    }

    async update(id, status) { //Upadte notes with ID to status
        return await Course.findByIdAndUpdate(
            id,
            {status},
            {new: true, runValidators: true}
        );
    }

    async delete(id) { //Delete courses with ID
        return await Course.findByIdAndDelete(id);
    }
}

module.exports = CourseDao;