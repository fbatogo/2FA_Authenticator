#include "totptests.h"

#include <QtTest>
#include "../otpimpl/totp.h"
#include "../otpimpl/sha1hash.h"
#include "../otpimpl/hmac.h"

#include <QDebug>

void totpTests::totpInvalidHmacTest()
{
    Totp invalidTotp;
    ByteArray invalidKey;

    QCOMPARE(invalidTotp.calculate(invalidKey, 20, 0), std::string(""));
}

void totpTests::totpTestCase1()
{
    std::shared_ptr<HashTypeBase> hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
    std::shared_ptr<Hmac> hmacToUse = std::shared_ptr<Hmac>(new Hmac(hashToUse));
    ByteArray key("12345678901234567890");
    Totp totp(hmacToUse);
    std::string result;

    result = totp.calculate(key, 59, 30, 8);
    qDebug("Expected : 94287082    Got : %s", result.c_str());
    QCOMPARE(std::string("94287082"), result);
}

void totpTests::totpTestCase2()
{
    std::shared_ptr<HashTypeBase> hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
    std::shared_ptr<Hmac> hmacToUse = std::shared_ptr<Hmac>(new Hmac(hashToUse));
    ByteArray key("12345678901234567890");
    Totp totp(hmacToUse);
    std::string result;

    result = totp.calculate(key, 1111111109, 30, 8);
    qDebug("Expected : 07081804    Got : %s", result.c_str());
    QCOMPARE(std::string("07081804"), result);
}

void totpTests::totpTestCase3()
{
    std::shared_ptr<HashTypeBase> hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
    std::shared_ptr<Hmac> hmacToUse = std::shared_ptr<Hmac>(new Hmac(hashToUse));
    ByteArray key("12345678901234567890");
    Totp totp(hmacToUse);
    std::string result;

    result = totp.calculate(key, 1111111111, 30, 8);
    qDebug("Expected : 14050471    Got : %s", result.c_str());
    QCOMPARE(std::string("14050471"), result);
}

void totpTests::totpTestCase4()
{
    std::shared_ptr<HashTypeBase> hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
    std::shared_ptr<Hmac> hmacToUse = std::shared_ptr<Hmac>(new Hmac(hashToUse));
    ByteArray key("12345678901234567890");
    Totp totp(hmacToUse);
    std::string result;

    result = totp.calculate(key, 1234567890, 30, 8);
    qDebug("Expected : 89005924    Got : %s", result.c_str());
    QCOMPARE(std::string("89005924"), result);
}

void totpTests::totpTestCase5()
{
    std::shared_ptr<HashTypeBase> hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
    std::shared_ptr<Hmac> hmacToUse = std::shared_ptr<Hmac>(new Hmac(hashToUse));
    ByteArray key("12345678901234567890");
    Totp totp(hmacToUse);
    std::string result;

    result = totp.calculate(key, 2000000000, 30, 8);
    qDebug("Expected : 69279037    Got : %s", result.c_str());
    QCOMPARE(std::string("69279037"), result);
}

void totpTests::totpTestCase6()
{
    std::shared_ptr<HashTypeBase> hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
    std::shared_ptr<Hmac> hmacToUse = std::shared_ptr<Hmac>(new Hmac(hashToUse));
    ByteArray key("12345678901234567890");
    Totp totp(hmacToUse);
    std::string result;

    result = totp.calculate(key, 20000000000, 30, 8);
    qDebug("Expected : 65353130    Got : %s", result.c_str());
    QCOMPARE(std::string("65353130"), result);
}

#if 0   // Test vectors from RFC6238 at https://tools.ietf.org/html/rfc6238
The test token shared secret uses the ASCII string value
   "12345678901234567890".  With Time Step X = 30, and the Unix epoch as
   the initial value to count time steps, where T0 = 0, the TOTP
   algorithm will display the following values for specified modes and
   timestamps.

  +-------------+--------------+------------------+----------+--------+
  |  Time (sec) |   UTC Time   | Value of T (hex) |   TOTP   |  Mode  |
  +-------------+--------------+------------------+----------+--------+
  |      59     |  1970-01-01  | 0000000000000001 | 94287082 |  SHA1  |
  |             |   00:00:59   |                  |          |        |
  |      59     |  1970-01-01  | 0000000000000001 | 46119246 | SHA256 |
  |             |   00:00:59   |                  |          |        |
  |      59     |  1970-01-01  | 0000000000000001 | 90693936 | SHA512 |
  |             |   00:00:59   |                  |          |        |
  |  1111111109 |  2005-03-18  | 00000000023523EC | 07081804 |  SHA1  |
  |             |   01:58:29   |                  |          |        |
  |  1111111109 |  2005-03-18  | 00000000023523EC | 68084774 | SHA256 |
  |             |   01:58:29   |                  |          |        |
  |  1111111109 |  2005-03-18  | 00000000023523EC | 25091201 | SHA512 |
  |             |   01:58:29   |                  |          |        |
  |  1111111111 |  2005-03-18  | 00000000023523ED | 14050471 |  SHA1  |
  |             |   01:58:31   |                  |          |        |
  |  1111111111 |  2005-03-18  | 00000000023523ED | 67062674 | SHA256 |
  |             |   01:58:31   |                  |          |        |
  |  1111111111 |  2005-03-18  | 00000000023523ED | 99943326 | SHA512 |
  |             |   01:58:31   |                  |          |        |
  |  1234567890 |  2009-02-13  | 000000000273EF07 | 89005924 |  SHA1  |
  |             |   23:31:30   |                  |          |        |
  |  1234567890 |  2009-02-13  | 000000000273EF07 | 91819424 | SHA256 |
  |             |   23:31:30   |                  |          |        |
  |  1234567890 |  2009-02-13  | 000000000273EF07 | 93441116 | SHA512 |
  |             |   23:31:30   |                  |          |        |
  |  2000000000 |  2033-05-18  | 0000000003F940AA | 69279037 |  SHA1  |
  |             |   03:33:20   |                  |          |        |
  |  2000000000 |  2033-05-18  | 0000000003F940AA | 90698825 | SHA256 |
  |             |   03:33:20   |                  |          |        |
  |  2000000000 |  2033-05-18  | 0000000003F940AA | 38618901 | SHA512 |
  |             |   03:33:20   |                  |          |        |
  | 20000000000 |  2603-10-11  | 0000000027BC86AA | 65353130 |  SHA1  |
  |             |   11:33:20   |                  |          |        |
  | 20000000000 |  2603-10-11  | 0000000027BC86AA | 77737706 | SHA256 |
  |             |   11:33:20   |                  |          |        |
  | 20000000000 |  2603-10-11  | 0000000027BC86AA | 47863826 | SHA512 |
  |             |   11:33:20   |                  |          |        |
  +-------------+--------------+------------------+----------+--------+
#endif // 0
