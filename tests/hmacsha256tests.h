#ifndef HMACSHA256TESTS_H
#define HMACSHA256TESTS_H

#include <QObject>
#include "autotest.h"

class HmacSha256Tests : public QObject
{
    Q_OBJECT

private slots:
    void hmacTestCase1();
    void hmacTestCase2();
    void hmacTestCase3();
    void hmacTestCase4();
    void hmacTestCase5();
    void hmacTestCase6();
    void hmacTestCase7();
};

DECLARE_TEST(HmacSha256Tests)

#endif // HMACSHA256TESTS_H
