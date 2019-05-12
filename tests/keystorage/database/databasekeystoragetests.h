#ifndef DATABASEKEYSTORAGETESTS_H
#define DATABASEKEYSTORAGETESTS_H

#include <QObject>

class DatabaseKeyStorageTests : public QObject
{
    Q_OBJECT

private slots:
    void storageIdTests();
    void e2eTests();
};

#endif // DATABASEKEYSTORAGETESTS_H
