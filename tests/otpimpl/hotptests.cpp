#include <testsuitebase.h>

#include "otpimpl/hotp.h"
#include "otpimpl/hmac.h"
#include "otpimpl/sha1hash.h"

#include <QDebug>

// Test vectors taken from RFC 4226 found at https://tools.ietf.org/html/rfc4226

/******
 *  Test values :
 *
 * The following test data uses the ASCII string
   "12345678901234567890" for the secret:

   Secret = 0x3132333435363738393031323334353637383930

   Table 1 details for each count, the intermediate HMAC value.

   Count    Hexadecimal HMAC-SHA-1(secret, count)
   0        cc93cf18508d94934c64b65d8ba7667fb7cde4b0
   1        75a48a19d4cbe100644e8ac1397eea747a2d33ab
   2        0bacb7fa082fef30782211938bc1c5e70416ff44
   3        66c28227d03a2d5529262ff016a1e6ef76557ece
   4        a904c900a64b35909874b33e61c5938a8e15ed1c
   5        a37e783d7b7233c083d4f62926c7a25f238d0316
   6        bc9cd28561042c83f219324d3c607256c03272ae
   7        a4fb960c0bc06e1eabb804e5b397cdc4b45596fa
   8        1b3c89f65e6c9e883012052823443f048b4332db
   9        1637409809a679dc698207310c8c7fc07290d9e5

   Table 2 details for each count the truncated values (both in
   hexadecimal and decimal) and then the HOTP value.

                     Truncated
   Count    Hexadecimal    Decimal        HOTP
   0        4c93cf18       1284755224     755224
   1        41397eea       1094287082     287082
   2         82fef30        137359152     359152
   3        66ef7655       1726969429     969429
   4        61c5938a       1640338314     338314
   5        33c083d4        868254676     254676
   6        7256c032       1918287922     287922
   7         4e5b397         82162583     162583
   8        2823443f        673399871     399871
   9        2679dc69        645520489     520489
   */

SIMPLE_TEST_SUITE(HotpTests, Hotp);

TEST_F(HotpTests, InvalidHotpTest)
{
    ByteArray key;
    std::shared_ptr<HashTypeBase> hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
    std::shared_ptr<Hmac> hmacToUse = std::shared_ptr<Hmac>(new Hmac(hashToUse));

    // Fails because the key is empty.
    EXPECT_TRUE(calculate(key, 20, 6, false).empty());

    key.append("testing");

    // Fails because no HMAC is set.
    EXPECT_TRUE(calculate(key, 20, 6, false).empty());

    setHmac(hmacToUse);

    // Fails because the length is less than 6.
    EXPECT_TRUE(calculate(key, 20, 5, false).empty());

    // Fails because the length is greater than 8.
    EXPECT_TRUE(calculate(key, 20, 9, false).empty());
}

TEST_F(HotpTests, HotpTest1)
{
    std::shared_ptr<HashTypeBase> hashToUse = std::shared_ptr<HashTypeBase>(new Sha1Hash());
    std::shared_ptr<Hmac> hmacToUse = std::shared_ptr<Hmac>(new Hmac(hashToUse));
    ByteArray secret("12345678901234567890");
    std::string hotpCalc;
    std::vector<std::string> expectedResults;

    setHmac(hmacToUse);

    // Build the list of expected results.
    expectedResults.clear();

    expectedResults.push_back("755224");
    expectedResults.push_back("287082");
    expectedResults.push_back("359152");
    expectedResults.push_back("969429");
    expectedResults.push_back("338314");
    expectedResults.push_back("254676");
    expectedResults.push_back("287922");
    expectedResults.push_back("162583");
    expectedResults.push_back("399871");
    expectedResults.push_back("520489");

    for (size_t i = 0; i < expectedResults.size(); i++) {
        hotpCalc = calculate(secret, i, 6);
        if (hotpCalc.empty()) {
            FAIL() << "Failed to generate an HOTP value!";
        }

        qDebug("HOTP calculated value %zu : %s ==? %s", i, hotpCalc.c_str(), expectedResults.at(i).c_str());
        EXPECT_EQ(hotpCalc, expectedResults.at(i));
    }
}
