QT += testlib
QT += widgets testlib

QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG += console
CONFIG -= app_bundle

CONFIG += c++11

QT += core gui widgets network multimedia
TARGET = tst_studenttest

TEMPLATE = app

SOURCES +=  \
    tst_studenttest.cc \
    ../Student/agent.cpp \
    ../Student/commonresource.cpp \
    ../Student/logic.cpp \


DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../Student/agent.hh \
    ../Student/commonresource.hh \
    ../Student/logic.hh \



#include "../Student/logic.hh"
#include "../Student/logic.cpp"


win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../course/Course/release/ -lCourse
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../course/Course/debug/ -lCourse
else:unix: LIBS += \
    -L$$OUT_PWD/../course/Course/ -lCourse


INCLUDEPATH += \
    $$PWD/../course/Course \
    $$PWD/../Student \
    $$PWD/Student

DEPENDPATH += \
    $$PWD/../course/Course \
    $$PWD/../Student

RESOURCES += \
    $$PWD/../course/Course/default_assets.qrc

