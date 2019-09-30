#include <testsuitebase.h>

#include "logger.h"

EMPTY_TEST_SUITE(LoggerTests);

TEST_F(LoggerTests, MiscTests)
{
    // Get the QML singleton.
    EXPECT_TRUE(nullptr != Logger::getQmlSingleton(nullptr, nullptr));

    Logger::getInstance()->log(QString("test qstring"));
    Logger::getInstance()->logWarning(QString("test warning qstring"));
}
