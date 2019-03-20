#ifndef SECRETDATABASETESTS_H
#define SECRETDATABASETESTS_H

#include <QObject>

class SecretDatabaseTests : public QObject
{
    Q_OBJECT

private slots:
    void openDatabaseTest();
    void closeDatabaseTest();
    void addDatabaseEntryKeyEntryTest();
    void getAllEntriesTest();
    void updateDatabaseEntryTest();

};


#endif // SECRETDATABASETESTS_H
