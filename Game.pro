TEMPLATE = subdirs

SUBDIRS += \
	StudentTest \
	course \
	Student

course.subdir = course/Course
Student.depends = course
StudentTest.depends = Student
