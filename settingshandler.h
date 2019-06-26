#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QSettings>

class SettingsHandler : public QObject
{
    Q_OBJECT

public:
    virtual ~SettingsHandler();

    static SettingsHandler *getInstance();
    static QObject *getQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE bool showHotpCounterValue();
    Q_INVOKABLE void setShowHotpCounterValue(bool newvalue);

    Q_INVOKABLE bool showIssuer();
    Q_INVOKABLE void setShowIssuer(bool newvalue);

    Q_INVOKABLE bool showHashAlgorithm();
    Q_INVOKABLE void setShowHashAlgorithm(bool newvalue);

    Q_INVOKABLE bool logToFile();
    Q_INVOKABLE void setLogToFile(bool newvalue);

    Q_INVOKABLE QString databaseLocation();
    Q_INVOKABLE void setDatabaseLocation(const QString &newLocation);

    QString dataPath();
    bool dataDirectoryExistsOrIsCreated();

private:
    SettingsHandler();

    bool mValid;

    bool mShowHotpCounter;
    bool mShowIssuer;
    bool mShowAlgorithm;
    bool mLogToFile;
    QString mDatabaseLocation;

    QSettings *mSettingsDatabase;
};

#endif // SETTINGSHANDLER_H
