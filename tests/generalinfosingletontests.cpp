#include "generalinfosingletontests.h"

#include <QtTest>

#include "generalinfosingleton.h"

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
