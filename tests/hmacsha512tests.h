#ifndef HMACSHA512TESTS_H
#define HMACSHA512TESTS_H

#include <QObject>
#include "autotest.h"

class HmacSha512Tests : public QObject
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

DECLARE_TEST(HmacSha512Tests)

#endif // HMACSHA512TESTS_H
