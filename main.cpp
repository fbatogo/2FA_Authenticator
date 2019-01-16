#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "interfacesingleton.h"
#include "keystorage/keyentry.h"
#include "uikeyentries.h"
#include "otp/otpentry.h"
#include "uiotpentries.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<InterfaceSingleton>("InterfaceSingleton", 1, 0, "InterfaceSingleton", InterfaceSingleton::getQmlSingleton);

    qmlRegisterType<KeyEntry>("KeyEntry", 1, 0, "KeyEntry");
    qmlRegisterType<UiKeyEntries>("UiKeyEntries", 1, 0, "UiKeyEntries");

    qmlRegisterType<OtpEntry>("OtpEntry", 1, 0, "OtpEntry");
    qmlRegisterType<UiOtpEntries>("UiOtpEntries", 1, 0, "UiOtpEntries");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
