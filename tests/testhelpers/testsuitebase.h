#ifndef TESTSUITEBASE_H
#define TESTSUITEBASE_H

#include <limits>
#include <gtest/gtest.h>

// Some macros for configuring tests.
#define SIMPLE_TEST_SUITE(a, b)  class a : public TestSuiteBase, public b \
            { \
            protected: \
                a() {}    \
            }

#define EMPTY_TEST_SUITE(a)  class a : public TestSuiteBase \
            { \
            protected: \
                a() {}    \
            }

class TestSuiteBase : public ::testing::Test
{
public:
    TestSuiteBase();
};

#endif // TESTSUITEBASE_H
