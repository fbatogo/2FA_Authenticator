#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "interfacesingleton.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<InterfaceSingleton>("InterfaceSingleton", 1, 0, "InterfaceSingleton", InterfaceSingleton::getQmlSingleton);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
