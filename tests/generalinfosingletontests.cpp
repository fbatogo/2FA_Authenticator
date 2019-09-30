#include <testsuitebase.h>

#include "generalinfosingleton.h"

EMPTY_TEST_SUITE(GeneralInfoSingletonTests);

TEST_F(GeneralInfoSingletonTests, SingletonTest)
{
    EXPECT_TRUE(GeneralInfoSingleton::getInstance() != nullptr);
    EXPECT_TRUE(GeneralInfoSingleton::getQmlSingleton(nullptr, nullptr) != nullptr);
}

TEST_F(GeneralInfoSingletonTests, VersionTest)
{
    QString result;

    result = GeneralInfoSingleton::getInstance()->version();
    EXPECT_TRUE(!result.isEmpty());
}

TEST_F(GeneralInfoSingletonTests, LicenseTest)
{
    QString result;

    result = GeneralInfoSingleton::getInstance()->licenseText();
    EXPECT_TRUE(!result.isEmpty());
}

TEST_F(GeneralInfoSingletonTests, HaveZBarTest)
{
#ifdef NO_ZBAR
    EXPECT_TRUE(!GeneralInfoSingleton::getInstance()->haveZBar());
#else
    EXPECT_TRUE(GeneralInfoSingleton::getInstance()->haveZBar());
#endif // NO_ZBAR
}

TEST_F(GeneralInfoSingletonTests, IsEncodedProperlyTest)
{
    // Shouldn't be encoded properly.
    EXPECT_TRUE(!GeneralInfoSingleton::getInstance()->isEncodedProperly(KEYENTRY_KEYTYPE_HEX, "nothexencoded"));
    EXPECT_TRUE(!GeneralInfoSingleton::getInstance()->isEncodedProperly(KEYENTRY_KEYTYPE_BASE32, "notbase32encoded"));
    EXPECT_TRUE(!GeneralInfoSingleton::getInstance()->isEncodedProperly(1024, "invalidencodingtype"));
}
