QT += quick sql multimedia svg

# If we aren't building on Windows, use the x11 extras.
!win32 {
    #DEFINES += NO_ZBAR
    QT += x11extras
} else {
    # On Windows, we don't currently support zbar.
    DEFINES *= NO_ZBAR

    #INCLUDEPATH += "C:/Program Files (x86)/ZBar/include"
    #LIBS += "C:/Program Files (x86)/ZBar/lib/libzbar-0.lib"
}

CONFIG += c++11

# If we are told to use the address sanitizer.
asan {
    QMAKE_CFLAGS += -fsanitize=address #-fsanitize-address-use-after-scope
    QMAKE_LFLAGS += -fsanitize=address
}

contains(QT, testlib) {
    message(Will use qDebug for logging...)
    DEFINES += USE_QDEBUG

    # Make sure we build against the QTest library.
    CONFIG += qtestlib

    QMAKE_CFLAGS += -ftest-coverage -fprofile-arcs
    QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
    QMAKE_LFLAGS += -ftest-coverage -fprofile-arcs

    # Include our test runner/helper.
    include(tests/QtTestRunner/QtTestRunner.pri)
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    keystorage/keyentry.cpp \
    keystorage/keystorage.cpp \
    logger.cpp \
    keystorage/database/databasekeystorage.cpp \
    otp/otphandler.cpp \
    uiclipboard.cpp \
    zbar/qrcodefilter.cpp \
    zbar/qrvideorunnable.cpp \
    zbar/qrcodestringparser.cpp \
    otpimpl/hotp.cpp \
    otpimpl/hmac.cpp \
    otpimpl/sha1impl.c \
    otpimpl/sha1hash.cpp \
    otpimpl/totp.cpp \
    otpimpl/base32coder.cpp \
    otpimpl/hexdecoder.cpp \
    otpimpl/sha256hash.cpp \
    otpimpl/sha512hash.cpp \
    otpimpl/sha2.c \
    settingshandler.cpp \
    keystorage/database/secretdatabase.cpp \
    generalinfosingleton.cpp \
    keyentriessingleton.cpp

HEADERS += \
    keystorage/database/secretdatabase.h \
    keystorage/keystoragebase.h \
    keystorage/keyentry.h \
    keystorage/keystorage.h \
    logger.h \
    keystorage/database/databasekeystorage.h \
    otp/otphandler.h \
    uiclipboard.h \
    zbar/qrcodefilter.h \
    zbar/qrvideorunnable.h \
    zbar/qrcodestringparser.h \
    otpimpl/hotp.h \
    otpimpl/hmac.h \
    otpimpl/sha1impl.h \
    otpimpl/hashtypebase.h \
    otpimpl/sha1hash.h \
    otpimpl/totp.h \
    otpimpl/base32coder.h \
    otpimpl/hexdecoder.h \
    zbar/myqzbarimage.h \
    otpimpl/sha256hash.h \
    otpimpl/sha512hash.h \
    otpimpl/sha2.h \
    settingshandler.h \
    generalinfosingleton.h \
    keyentriessingleton.h

RESOURCES += qml.qrc

win32 {
#    INCLUDEPATH += "C:/Program Files (x86)/ZBar/include"
#    LIBS += "C:/Program Files (x86)/ZBar/lib/libzbar-0.lib"
} else {
    # Link to zbar.
    CONFIG += link_pkgconfig
    PKGCONFIG += zbar
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =


# If we are building in the unit tests, we need to tweak a few files.
message($$QT)
contains(QT, testlib) {
    # Include the test source files.
    include(tests/tests.pri)

    CONFIG -= qml_debug
} else {
    # Use our 'normal' main.cpp in the build.
    SOURCES += main.cpp
}

# Uncomment and rebuild to use address sanitizer.
#QMAKE_CFLAGS += -fsanitize=address -fno-omit-frame-pointer
#LIBS += -lasan

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/licenses.txt

