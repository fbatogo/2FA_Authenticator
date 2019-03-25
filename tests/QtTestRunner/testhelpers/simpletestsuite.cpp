#include "simpletestsuite.h"

#include <QtTest>

void SimpleTestSuite::simpleTest()
{
    QVERIFY(true);      // Have a simple test, to make things complete.
}

void SimpleTestSuite::failingTest()
{
    QVERIFY(false);     // Cause the test to fail.
}
