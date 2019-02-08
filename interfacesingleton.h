#ifndef INTERFACESINGLETON_H
#define INTERFACESINGLETON_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>

#include "keystorage/keystorage.h"
#include "keystorage/keyentry.h"
#include "uikeyentries.h"
#include "otp/otpentry.h"
#include "uiotpentries.h"

class InterfaceSingleton : public QObject
{
    Q_OBJECT

public:
    virtual ~InterfaceSingleton();

    static InterfaceSingleton *getInstance();
    static QObject *getQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    // Allow the QML code to write to the logger.
    Q_INVOKABLE void logDebug(QString logline);
    Q_INVOKABLE void logError(QString logline);

    Q_INVOKABLE QString version();
    Q_INVOKABLE UiKeyEntries *keyEntries();
    Q_INVOKABLE UiOtpEntries *otpEntries();
    Q_INVOKABLE bool addKeyEntry(QString identifier, QString secret, int keyType, int otpType, int numberCount);
    Q_INVOKABLE bool deleteKey(QString identifier);

private:
    InterfaceSingleton();

    void updateEngine(QQmlEngine *engine);

    KeyStorage mKeyStorage;

    QQmlEngine *mQmlEngine;
};

#endif // INTERFACESINGLETON_H
