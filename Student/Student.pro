TEMPLATE = app
TARGET = Game

QT += core gui widgets network multimedia

CONFIG += c++14

SOURCES += \
    actioncard.cpp \
    agent.cpp \
    agentdialog.cpp \
    agentitem.cpp \
    cardpaymentaction.cpp \
    commonresource.cpp \
    carditem.cpp \
    gamerunner.cpp \
    gamescene.cpp \
    gamewindow.cpp \
    gamesetup.cpp \
    locationitem.cpp \
    logic.cpp \
    main.cc \
    mapitem.cpp \
    playerhand.cpp \
    popupdialog.cpp \
    scenearrow.cpp \
    sendagentaction.cpp \
    withdrawagentaction.cpp

win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../course/Course/release/ -lCourse
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../course/Course/debug/ -lCourse
else:unix: LIBS += \
    -L$$OUT_PWD/../course/Course/ -lCourse

INCLUDEPATH += \
    $$PWD/../course/Course

DEPENDPATH += \
    $$PWD/../course/Course

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../course/Course/release/libCourse.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../course/Course/debug/libCourse.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../course/Course/release/Course.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../course/Course/debug/Course.lib
else:unix: PRE_TARGETDEPS += \
    $$OUT_PWD/../course/Course/libCourse.a

HEADERS += \
    actioncard.hh \
    agent.hh \
    agentdialog.hh \
    agentitem.hh \
    cardpaymentaction.hh \
    commonresource.hh \
    carditem.hh \
    gamerunner.hh \
    gamescene.hh \
    gamewindow.hh \
    gamesetup.hh \
    locationitem.hh \
    logic.hh \
    mapitem.hh \
    playerhand.hh \
    popupdialog.hh \
    scenearrow.hh \
    sendagentaction.hh \
    withdrawagentaction.hh

FORMS += \
    agentdialog.ui \
    gamewindow.ui \
    popupdialog.ui

RESOURCES += \
    default.jpg

DISTFILES += \
    defaultsettings.dat
