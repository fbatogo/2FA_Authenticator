# This file adds the necessary source files for running unit tests, and tweaks the build
# to reach that end.

message(Building unit test binary...)

# Add the extra source files that we use for testing.
HEADERS += \
    $$PWD/secretdatabasetests.h \
    $$PWD/qrcodestringparsertests.h \
    $$PWD/autotest.h \
    $$PWD/sha1tests.h \
    $$PWD/hmacsha1tests.h

SOURCES += \
    $$PWD/secretdatabasetests.cpp \
    $$PWD/qrcodestringparsertests.cpp \
    $$PWD/sha1tests.cpp \
    $$PWD/hmacsha1tests.cpp


# And, add our testmain.cpp as the 'main' that we want to use.
SOURCES += $$PWD/testmain.cpp
