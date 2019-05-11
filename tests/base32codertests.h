#ifndef BASE32CODERTESTS_H
#define BASE32CODERTESTS_H

#include <QObject>
#include <vector>
#include <string>

#include "container/bytearray.h"
#include "../otpimpl/base32coder.h"

class Base32CoderTests : public QObject
{
    Q_OBJECT

private slots:
    void encoderTest1();
    void decoderTest1();
    void isBase32EncodedTests();
    void negativeTests();

private:
    std::vector<ByteArray> getClearTextTests();
    std::vector<ByteArray> getEncodedTextTests();
};

class Base32CoderProxy : public Base32Coder
{
public:
    bool decode8CharsProxy(const ByteArray &data, size_t dataOffset, ByteArray &target) {
        return decode8Chars(data, dataOffset, target);
    }
};

#endif // BASE32DECODERTESTS_H
