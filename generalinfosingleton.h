#ifndef GENERALINFOSINGLETON_H
#define GENERALINFOSINGLETON_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>

#include "keyentriessingleton.h"

const QString APP_VERSION="0.01";

class GeneralInfoSingleton : public QObject
{
    Q_OBJECT

public:
    virtual ~GeneralInfoSingleton();

    static GeneralInfoSingleton *getInstance();
    static QObject *getQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE QString version();
    Q_INVOKABLE QString licenseText();

    Q_INVOKABLE bool isEncodedProperly(int encodingType, const QString &valueToCheck);

    Q_INVOKABLE bool haveZBar();

private:
    GeneralInfoSingleton();

    void updateEngine(QQmlEngine *engine);

    QQmlEngine *mQmlEngine;
};

#endif // GENERALINFOSINGLETON_H
