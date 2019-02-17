#ifndef BASE32CODERTESTS_H
#define BASE32CODERTESTS_H

#include <QObject>
#include "autotest.h"
#include <vector>
#include <string>

class Base32CoderTests : public QObject
{
    Q_OBJECT

private slots:
    void encoderTest1();
    void decoderTest1();
    void isBase32EncodedTests();

private:
    std::vector<std::string> getClearTextTests();
    std::vector<std::string> getEncodedTextTests();
};

DECLARE_TEST(Base32CoderTests)

#endif // BASE32DECODERTESTS_H
