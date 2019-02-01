#ifndef SHA1TESTS_H
#define SHA1TESTS_H

#include <string>
#include <QObject>
#include "autotest.h"

class Sha1Tests : public QObject
{
    Q_OBJECT

private slots:
    void sha1Tests1();
    void sha1Tests2();

private:
    std::string binaryToString(unsigned char *bytes, size_t bytesLength);

};

DECLARE_TEST(Sha1Tests)

#endif // SHA1TESTS_H
