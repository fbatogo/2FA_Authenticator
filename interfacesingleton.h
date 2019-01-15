#ifndef INTERFACESINGLETON_H
#define INTERFACESINGLETON_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>

#include "keystorage/keyentry.h"

class InterfaceSingleton : public QObject
{
    Q_OBJECT

public:
    virtual ~InterfaceSingleton() = default;

    static InterfaceSingleton *getInstance();
    static QObject *getQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    void updateEngine(QQmlEngine *engine);

    Q_INVOKABLE QString version();
    Q_INVOKABLE QList<KeyEntry> keyEntries();

private:
    InterfaceSingleton();

    QQmlEngine *mQmlEngine;
};

#endif // INTERFACESINGLETON_H
