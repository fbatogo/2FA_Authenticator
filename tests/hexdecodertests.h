#ifndef HEXDECODERTESTS_H
#define HEXDECODERTESTS_H

#include <QObject>

#include "../otpimpl/hexdecoder.h"

class HexDecoderTests : public QObject
{
    Q_OBJECT

private slots:
    void hexDecoderTest1();
    void isHexEncodedTest();
    void decodeInvalidTextTest();
    void negativeTests();
};

class HexDecoderProxy : public HexDecoder
{
public:
    unsigned char decodeOneByteProxy(const std::string &oneByte) {
        return decodeOneByte(oneByte);
    }

    unsigned char decodeOneNibbleProxy(char oneNibble) {
        return decodeOneNibble(oneNibble);
    }
};

#endif // HEXDECODERTESTS_H
