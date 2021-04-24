QT += testlib
#QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

QT += core gui widgets network multimedia
TARGET = tst_studenttest

TEMPLATE = app

SOURCES +=  tst_studenttest.cpp \

DEFINES += SRCDIR=\\\"$$PWD/\\\"


win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../course/Course/release/ -lCourse
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../course/Course/debug/ -lCourse
else:unix: LIBS += \
    -L$$OUT_PWD/../course/Course/ -lCourse


INCLUDEPATH += \
    $$PWD/../course/Course \
    $$PWD/../Student

DEPENDPATH += \
    $$PWD/../course/Course \
    $$PWD/../Student

