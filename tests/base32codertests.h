#ifndef BASE32CODERTESTS_H
#define BASE32CODERTESTS_H

#include <QObject>
#include <vector>
#include <string>

#include "container/bytearray.h"

class Base32CoderTests : public QObject
{
    Q_OBJECT

private slots:
    void encoderTest1();
    void decoderTest1();
    void isBase32EncodedTests();

private:
    std::vector<ByteArray> getClearTextTests();
    std::vector<ByteArray> getEncodedTextTests();
};

//DECLARE_TEST(Base32CoderTests)

#endif // BASE32DECODERTESTS_H
