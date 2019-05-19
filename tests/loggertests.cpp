#include "loggertests.h"

#include <QtTest>
#include "logger.h"

void LoggerTests::miscTests()
{
    // Get the QML singleton.
    QVERIFY(nullptr != Logger::getQmlSingleton(nullptr, nullptr));

    Logger::getInstance()->log(QString("test qstring"));
    Logger::getInstance()->logWarning(QString("test warning qstring"));
}
