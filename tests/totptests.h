#ifndef TOTPTESTS_H
#define TOTPTESTS_H

#include <QObject>
#include "autotest.h"

class totpTests : public QObject
{
    Q_OBJECT

private slots:
    void totpTestCase1();
    void totpTestCase2();
    void totpTestCase3();
    void totpTestCase4();
    void totpTestCase5();
    void totpTestCase6();
};

DECLARE_TEST(totpTests)

#endif // TOTPTESTS_H