#ifndef HOTPTESTS_H
#define HOTPTESTS_H

#include <QObject>
#include "autotest.h"

class hotpTests : public QObject
{
    Q_OBJECT

private slots:
    void hotpTest1();
};

DECLARE_TEST(hotpTests)

#endif // HOTPTESTS_H
