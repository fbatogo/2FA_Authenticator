#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "interfacesingleton.h"
#include "keystorage/keyentry.h"
#include "uikeyentries.h"
#include "uiclipboard.h"
#include "zbar/qrcodefilter.h"
#include "zbar/qrcodestringparser.h"
#include "settingshandler.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<InterfaceSingleton>("Rollin.InterfaceSingleton", 1, 0, "InterfaceSingleton", InterfaceSingleton::getQmlSingleton);

    qmlRegisterSingletonType<QRCodeStringParser>("Rollin.QRCodeSingleton", 1, 0, "QRCodeSingleton", QRCodeStringParser::getQmlSingleton);

    qmlRegisterSingletonType<SettingsHandler>("Rollin.SettingsHandler", 1, 0, "SettingsHandler", SettingsHandler::getQmlSingleton);

    qmlRegisterSingletonType<SettingsHandler>("Rollin.Logger", 1, 0, "Log", Logger::getQmlSingleton);

    qmlRegisterType<KeyEntry>("KeyEntry", 1, 0, "KeyEntry");
    qmlRegisterType<UiKeyEntries>("UiKeyEntries", 1, 0, "UiKeyEntries");

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
