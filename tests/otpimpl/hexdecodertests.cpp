#include <testsuitebase.h>

#include <vector>
#include <string>
#include <QString>

#include "otpimpl/hexdecoder.h"
#include "testutils.h"

SIMPLE_TEST_SUITE(HexDecoderTests, HexDecoder);

// Test a few different hex decodes.
TEST_F(HexDecoderTests, HexDecoderTest1)
{
    unsigned char lowerResult[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char upperResult[] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0};
    std::vector<std::string> hexToDecode;
    std::vector<unsigned char *> decodedHex;
    HexDecoder decoder;
    ByteArray result;
    ByteArray baToDecode;

    // Set up our hex tests.
    hexToDecode.push_back("");

    // Lower nibble tests.
    hexToDecode.push_back("0102030405060708090A0B0C0D0E0F");
    hexToDecode.push_back("01 02 03 04 05 06 07 08 09 0A 0b 0C 0d 0E 0f");
    hexToDecode.push_back("01.02.03.04.05.06.07.08.09.0A.0b.0c.0d.0e.0f");
    hexToDecode.push_back("01-02-03-04-05-06-07-08-09-0A-0B-0c-0d-0e-0f");
    hexToDecode.push_back("01:02:03:04:05:06:07:08:09:0A:0B:0c:0d:0e:0f");
    hexToDecode.push_back("0x010x020x030x040x050x060x070x080x090x0a0x0b0x0c0x0d0x0e0x0f");
    hexToDecode.push_back("0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f");
    hexToDecode.push_back("010203.040506.070809.0a0b0c.0d0e0f");

    // Upper nibble tests.
    hexToDecode.push_back("102030405060708090A0B0c0D0e0f0");        // Lower case c isn't a typo!

    // Set up our decoded values.
    decodedHex.push_back(nullptr);

    // Lower nibble results.
    decodedHex.push_back((unsigned char *)&lowerResult);
    decodedHex.push_back((unsigned char *)&lowerResult);
    decodedHex.push_back((unsigned char *)&lowerResult);
    decodedHex.push_back((unsigned char *)&lowerResult);
    decodedHex.push_back((unsigned char *)&lowerResult);
    decodedHex.push_back((unsigned char *)&lowerResult);
    decodedHex.push_back((unsigned char *)&lowerResult);
    decodedHex.push_back((unsigned char *)&lowerResult);

    // Upper nibble results.
    decodedHex.push_back((unsigned char *)&upperResult);

    // Make sure that both lists are the same size.
    EXPECT_EQ(hexToDecode.size(), decodedHex.size());

    for (size_t i = 0; i < hexToDecode.size(); i++) {
        // Decode the text.
        baToDecode.fromStdString(hexToDecode.at(i));
        result = decoder.decode(baToDecode);

        if (i != 0) {
            // Make sure the size is 0x0f octets.
            EXPECT_EQ(result.size(), (unsigned long)0x0f);
        }

        qDebug("(Test %d) Original String : %s", (int)i, hexToDecode.at(i).c_str());
        qDebug("(Test %d) Decoded String  : %s", (int)i, TestUtils::binaryToString(result).c_str());

        if (memcmp(result.toUCharArrayPtr(), decodedHex.at(i), result.size()) != 0) {
            qFatal("Test %d failed!", (int)i);
            FAIL() << "Test " + QString::number(i).toStdString() + " failed!";
        }
    }
}

TEST_F(HexDecoderTests, IsHexEncodedTest)
{
    EXPECT_TRUE(HexDecoder::isHexEncoded("0102030405060708090A0B0C0D0E0F"));
    EXPECT_TRUE(HexDecoder::isHexEncoded("01 02 03 04 05 06 07 08 09 0A 0b 0C 0d 0E 0f"));
    EXPECT_TRUE(HexDecoder::isHexEncoded("01.02.03.04.05.06.07.08.09.0A.0b.0c.0d.0e.0f"));
    EXPECT_TRUE(HexDecoder::isHexEncoded("01-02-03-04-05-06-07-08-09-0A-0B-0c-0d-0e-0f"));
    EXPECT_TRUE(HexDecoder::isHexEncoded("01:02:03:04:05:06:07:08:09:0A:0B:0c:0d:0e:0f"));
    EXPECT_TRUE(HexDecoder::isHexEncoded("0x010x020x030x040x050x060x070x080x090x0a0x0b0x0c0x0d0x0e0x0f"));
    EXPECT_TRUE(HexDecoder::isHexEncoded("0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0a 0x0b 0x0c 0x0d 0x0e 0x0f"));
    EXPECT_TRUE(HexDecoder::isHexEncoded("010203.040506.070809.0a0b0c.0d0e0f"));
    EXPECT_TRUE(HexDecoder::isHexEncoded("102030405060708090A0B0c0D0e0f0"));        // Lower case c isn't a typo!

    // Then, some negative tests.
    EXPECT_TRUE(!HexDecoder::isHexEncoded(""));
    EXPECT_TRUE(!HexDecoder::isHexEncoded("nothexencoded"));
    EXPECT_TRUE(!HexDecoder::isHexEncoded("deadbeef is good"));
    EXPECT_TRUE(!HexDecoder::isHexEncoded("deadbeefz"));
}

TEST_F(HexDecoderTests, DecodeInvalidTextTest)
{
    ByteArray testArray("abc");
    ByteArray result;
    HexDecoder decoder;

    result = decoder.decode(testArray);
    EXPECT_TRUE(result.empty());
}

TEST_F(HexDecoderTests, NegativeTests)
{
    EXPECT_EQ((unsigned char)0x00, decodeOneByte("a"));
    EXPECT_EQ((unsigned char)0xff, decodeOneNibble('z'));
}
