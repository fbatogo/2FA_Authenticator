#ifndef SHA512TESTS_H
#define SHA512TESTS_H

#include <QObject>
#include "autotest.h"

class Sha512Tests : public QObject
{
    Q_OBJECT

private slots:
    void sha512ImplTests();
};

DECLARE_TEST(Sha512Tests)

#endif // SHA512TESTS_H
