#ifndef SETTINGSHANDLERTESTS_H
#define SETTINGSHANDLERTESTS_H

#include <QObject>

class SettingsHandlerTests : public QObject
{
    Q_OBJECT

private slots:
    void getSetSettingsTests();
    void databasePathLocationTests();
    void databaseFileLocationTests();
};

#endif // SETTINGSHANDLERTESTS_H
