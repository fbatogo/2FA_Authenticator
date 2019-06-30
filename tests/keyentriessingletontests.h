#ifndef KEYENTRIESSINGLETONTESTS_H
#define KEYENTRIESSINGLETONTESTS_H

#include <QObject>

class KeyEntriesSingletonTests : public QObject
{
    Q_OBJECT

private slots:
    void e2eTests();
    void openCloseTests();
};

#endif // KEYENTRIESSINGLETONTESTS_H
