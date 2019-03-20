#ifndef HOTPTESTS_H
#define HOTPTESTS_H

#include <QObject>

class hotpTests : public QObject
{
    Q_OBJECT

private slots:
    void invalidHotpTest();
    void hotpTest1();
};

//DECLARE_TEST(hotpTests)

#endif // HOTPTESTS_H
