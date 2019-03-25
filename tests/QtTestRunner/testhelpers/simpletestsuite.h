#ifndef SIMPLETESTSUITE_H
#define SIMPLETESTSUITE_H

#include <QObject>

class SimpleTestSuite : public QObject
{
    Q_OBJECT

private slots:
    void simpleTest();
};

#endif // SIMPLETESTSUITE_H
