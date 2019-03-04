#ifndef HMACSHA1TESTS_H
#define HMACSHA1TESTS_H

#include <QObject>
#include "autotest.h"

class HmacSha1Tests : public QObject
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

private:
    std::string binaryToString(unsigned char *bytes, size_t bytesLength);
};

//DECLARE_TEST(HmacSha1Tests)

#endif // HMACSHA1TESTS_H
