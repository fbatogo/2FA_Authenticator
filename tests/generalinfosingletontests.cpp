#include "generalinfosingletontests.h"

#include <QtTest>

#include "generalinfosingleton.h"

void GeneralInfoSingletonTests::singletonsTest()
{
    QVERIFY(GeneralInfoSingleton::getInstance() != nullptr);
    QVERIFY(GeneralInfoSingleton::getQmlSingleton(nullptr, nullptr) != nullptr);
}

void GeneralInfoSingletonTests::versionTest()
{
    QString result;

    result = GeneralInfoSingleton::getInstance()->version();
    QVERIFY(!result.isEmpty());
}

void GeneralInfoSingletonTests::licenseTest()
{
    QString result;

    result = GeneralInfoSingleton::getInstance()->licenseText();
    QVERIFY(!result.isEmpty());
}

void GeneralInfoSingletonTests::haveZBarTest()
{
#ifdef NO_ZBAR
    QVERIFY(!GeneralInfoSingleton::getInstance()->haveZBar());
#else
    QVERIFY(GeneralInfoSingleton::getInstance()->haveZBar());
#endif // NO_ZBAR
}

void GeneralInfoSingletonTests::isEncodedProperlyTest()
{
    // Shouldn't be encoded properly.
    QVERIFY(!GeneralInfoSingleton::getInstance()->isEncodedProperly(KEYENTRY_KEYTYPE_HEX, "nothexencoded"));
    QVERIFY(!GeneralInfoSingleton::getInstance()->isEncodedProperly(KEYENTRY_KEYTYPE_BASE32, "notbase32encoded"));
    QVERIFY(!GeneralInfoSingleton::getInstance()->isEncodedProperly(1024, "invalidencodingtype"));
}
