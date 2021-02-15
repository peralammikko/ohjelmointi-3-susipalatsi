TEMPLATE = subdirs

SUBDIRS += \
	course \
	Student

course.subdir = course/Course
Student.depends = course
