# This file adds the necessary source files for running unit tests, and tweaks the build
# to reach that end.

message(Building unit test binary...)

# Add the extra source files that we use for testing.
HEADERS += \
    $$PWD/container/bytearraytests.h \
    $$PWD/generalinfosingletontests.h \
    $$PWD/keyentriessingletontests.h \
    $$PWD/keystorage/database/databasekeystoragetests.h \
    $$PWD/keystorage/keystoragetests.h \
    $$PWD/loggertests.h \
    $$PWD/otp/otphandlertests.h \
    $$PWD/secretdatabasetests.h \
    $$PWD/qrcodestringparsertests.h \
    $$PWD/settingshandlertests.h \
    $$PWD/sha1tests.h \
    $$PWD/hmacsha1tests.h \
    $$PWD/testutils.h \
    $$PWD/hotptests.h \
    $$PWD/totptests.h \
    $$PWD/base32codertests.h \
    $$PWD/hexdecodertests.h \
    $$PWD/sha256tests.h \
    $$PWD/sha512tests.h \
    $$PWD/hmacsha256tests.h \
    $$PWD/hmacsha512tests.h \
    $$PWD/uiclipboardtests.h

SOURCES += \
    $$PWD/container/bytearraytests.cpp \
    $$PWD/generalinfosingletontests.cpp \
    $$PWD/keyentriessingletontests.cpp \
    $$PWD/keystorage/database/databasekeystoragetests.cpp \
    $$PWD/keystorage/keystoragetests.cpp \
    $$PWD/loggertests.cpp \
    $$PWD/otp/otphandlertests.cpp \
    $$PWD/secretdatabasetests.cpp \
    $$PWD/qrcodestringparsertests.cpp \
    $$PWD/settingshandlertests.cpp \
    $$PWD/sha1tests.cpp \
    $$PWD/hmacsha1tests.cpp \
    $$PWD/testutils.cpp \
    $$PWD/hotptests.cpp \
    $$PWD/totptests.cpp \
    $$PWD/base32codertests.cpp \
    $$PWD/hexdecodertests.cpp \
    $$PWD/sha256tests.cpp \
    $$PWD/sha512tests.cpp \
    $$PWD/hmacsha256tests.cpp \
    $$PWD/hmacsha512tests.cpp \
    $$PWD/main.cpp \
    $$PWD/uiclipboardtests.cpp


# And, add our testmain.cpp as the 'main' that we want to use.
SOURCES +=
