#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "generalinfosingleton.h"
#include "keystorage/keyentry.h"
#include "keyentriessingleton.h"
#include "uiclipboard.h"
#include "zbar/qrcodefilter.h"
#include "zbar/qrcodestringparser.h"
#include "settingshandler.h"
#include "logger.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Need the environment setting below or the timer animation runs too fast.
    qputenv("QSG_RENDER_LOOP", "basic");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Set the global Qt application settings values.
    app.setOrganizationName("Rollin' Organization");
    app.setOrganizationDomain("rollin.local");
    app.setApplicationName("Rollin'");

    qmlRegisterSingletonType<GeneralInfoSingleton>("Rollin.GeneralInfoSingleton", 1, 0, "GeneralInfoSingleton", GeneralInfoSingleton::getQmlSingleton);
    qmlRegisterSingletonType<KeyEntriesSingleton>("Rollin.KeyEntriesSingleton", 1, 0, "KeyEntriesSingleton", KeyEntriesSingleton::getQmlSingleton);

#ifndef NO_ZBAR
    qmlRegisterSingletonType<QRCodeStringParser>("Rollin.QRCodeSingleton", 1, 0, "QRCodeSingleton", QRCodeStringParser::getQmlSingleton);
#endif // NO_ZBAR

    qmlRegisterSingletonType<SettingsHandler>("Rollin.SettingsHandler", 1, 0, "SettingsHandler", SettingsHandler::getQmlSingleton);

    qmlRegisterSingletonType<SettingsHandler>("Rollin.Logger", 1, 0, "Log", Logger::getQmlSingleton);

    qmlRegisterSingletonType<SettingsHandler>("Rollin.Utils", 1, 0, "Utils", Utils::getQmlSingleton);

    qmlRegisterType<KeyEntry>("KeyEntry", 1, 0, "KeyEntry");

    qmlRegisterType<UiClipboard>("UiClipboard", 1, 0, "UiClipboard");

#ifndef NO_ZBAR
    qmlRegisterType<QRCodeFilter>("QRFilter", 1, 0, "QRFilter");
#endif // NO_ZBAR

    // One more Qt application setting value.
    app.setApplicationVersion(GeneralInfoSingleton::getInstance()->version());

    engine.load(QUrl(QStringLiteral("qrc:/resources/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
