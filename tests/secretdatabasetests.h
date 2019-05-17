#ifndef SECRETDATABASETESTS_H
#define SECRETDATABASETESTS_H

#include <QObject>
#include "../keystorage/database/secretdatabase.h"

class SecretDatabaseTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void addDatabaseEntryKeyEntryTest();
    void getAllEntriesTest();
    void updateDatabaseEntryTest();
    void deleteDatabaseEntryTest();

private:
    SecretDatabase testDatabase;
};


#endif // SECRETDATABASETESTS_H
