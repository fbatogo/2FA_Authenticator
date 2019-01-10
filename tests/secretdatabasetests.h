#ifndef SECRETDATABASETESTS_H
#define SECRETDATABASETESTS_H

#include <QObject>
#include "../keystorage/database/secretdatabase.h"

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
    void addDatabaseEntryKeyEntryTest();
    void getAllEntriesTest();

    SecretDatabase mTestDatabase;
};

#endif // SECRETDATABASETESTS_H
