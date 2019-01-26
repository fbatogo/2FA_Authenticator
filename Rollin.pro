QT += quick sql multimedia
CONFIG += c++11

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
    keystorage/database/secretdatabase.cpp \
    keystorage/keyentry.cpp \
    keystorage/keystorage.cpp \
    logger.cpp \
    keystorage/database/databasekeystorage.cpp \
    interfacesingleton.cpp \
    uikeyentries.cpp \
    otp/otpentry.cpp \
    uiotpentries.cpp \
    otp/otphandler.cpp \
    uiclipboard.cpp \
    zbar/qrcodefilter.cpp \
    zbar/qrvideorunnable.cpp

HEADERS += \
    keystorage/database/secretdatabase.h \
    keystorage/keystoragebase.h \
    keystorage/keyentry.h \
    keystorage/keystorage.h \
    logger.h \
    keystorage/database/databasekeystorage.h \
    interfacesingleton.h \
    uikeyentries.h \
    otp/otpentry.h \
    uiotpentries.h \
    otp/otphandler.h \
    uiclipboard.h \
    zbar/qrcodefilter.h \
    zbar/qrvideorunnable.h

RESOURCES += qml.qrc

!win32 {
# Link to liboath.
CONFIG += link_pkgconfig
PKGCONFIG += liboath zbar

LIBS += /usr/lib/x86_64-linux-gnu/libzbarqt.a
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
} else {
    # Use our 'normal' main.cpp in the build.
    SOURCES += main.cpp
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/licenses.txt

