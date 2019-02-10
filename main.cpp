#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "interfacesingleton.h"
#include "keystorage/keyentry.h"
#include "uikeyentries.h"
#include "otp/otpentry.h"
#include "uiotpentries.h"
#include "uiclipboard.h"
#include "zbar/qrcodefilter.h"
#include "zbar/qrcodestringparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

#ifndef FULLSCREEN
    QQmlApplicationEngine engine;
#endif // FULLSCREEN

    qmlRegisterSingletonType<InterfaceSingleton>("InterfaceSingleton", 1, 0, "InterfaceSingleton", InterfaceSingleton::getQmlSingleton);
    qmlRegisterSingletonType<QRCodeStringParser>("QRCodeSingleton", 1, 0, "QRCodeSingleton", QRCodeStringParser::getQmlSingleton);

    qmlRegisterType<KeyEntry>("KeyEntry", 1, 0, "KeyEntry");
    qmlRegisterType<UiKeyEntries>("UiKeyEntries", 1, 0, "UiKeyEntries");

    qmlRegisterType<OtpEntry>("OtpEntry", 1, 0, "OtpEntry");
    qmlRegisterType<UiOtpEntries>("UiOtpEntries", 1, 0, "UiOtpEntries");

    qmlRegisterType<UiClipboard>("UiClipboard", 1, 0, "UiClipboard");

#ifndef NO_ZBAR
    qmlRegisterType<QRCodeFilter>("QRFilter", 1, 0, "QRFilter");
#endif // NO_ZBAR

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
