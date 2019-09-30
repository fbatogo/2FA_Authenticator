#include "testsuitebase.h"

#include <logger.h>

TestSuiteBase::TestSuiteBase()
{
    std::string testName;

    testName = std::string(::testing::UnitTest::GetInstance()->current_test_info()->test_suite_name()) +
            "-" + std::string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + ".log";

    // Configure the logger to create a log file for each test case that contains
    // the name of the test suite and the name of the test case.
    Logger::getInstance()->setLogFile(QString::fromStdString(testName));
}
