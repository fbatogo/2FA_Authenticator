# This file adds the necessary source files for running unit tests, and tweaks the build
# to reach that end.

message(Building unit test binary...)

INCLUDEPATH += $$PWD/testhelpers

# Add the extra source files that we use for testing.
HEADERS += \
    $$PWD/testhelpers/testsuitebase.h \
    $$PWD/testhelpers/testutils.h

SOURCES += \
    $$PWD/container/bytearraytests.cpp \
    $$PWD/generalinfosingletontests.cpp \
    $$PWD/keyentriessingletontests.cpp \
    $$PWD/keystorage/database/databasekeystoragetests.cpp \
    $$PWD/keystorage/database/secretdatabasetests.cpp \
    $$PWD/keystorage/keystoragetests.cpp \
    $$PWD/loggertests.cpp \
    $$PWD/otp/otphandlertests.cpp \
    $$PWD/otpimpl/base32codertests.cpp \
    $$PWD/otpimpl/hexdecodertests.cpp \
    $$PWD/otpimpl/hmacsha1tests.cpp \
    $$PWD/otpimpl/hmacsha256tests.cpp \
    $$PWD/otpimpl/hmacsha512tests.cpp \
    $$PWD/otpimpl/hotptests.cpp \
    $$PWD/otpimpl/sha1tests.cpp \
    $$PWD/otpimpl/sha256tests.cpp \
    $$PWD/otpimpl/sha512tests.cpp \
    $$PWD/otpimpl/totptests.cpp \
    $$PWD/settingshandlertests.cpp \
    $$PWD/testhelpers/testsuitebase.cpp \
    $$PWD/testhelpers/testutils.cpp \
    $$PWD/uiclipboardtests.cpp \
    $$PWD/utilstests.cpp \
    $$PWD/zbar/qrcodestringparsertests.cpp


# And, add our testmain.cpp as the 'main' that we want to use.
SOURCES +=
