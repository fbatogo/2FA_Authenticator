#ifndef SECRETDATABASETESTS_H
#define SECRETDATABASETESTS_H

#include <QObject>
#include <secretdatabase.h>

class SecretDatabaseTests : public QObject
{
    Q_OBJECT

private slots:
    void databaseTests();

private:
    void removeOldDatabaseFile();
    void openDatabaseTest();
    void closeDatabaseTest();
    void addDatabaseEntryDiscreteTest();
    void addDatabaseEntrySecretEntryTest();
    void getAllEntriesTest();

    SecretDatabase mTestDatabase;
};

#endif // SECRETDATABASETESTS_H
