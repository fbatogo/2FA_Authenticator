TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

QT += widgets testlib

include (QtTestRunner.pri)

SOURCES += \
        main.cpp \
    testhelpers/simpletestsuite.cpp

HEADERS += \
    testhelpers/simpletestsuite.h
