#include "bytearraytests.h"

#include "container/bytearray.h"
#include <QtTest>

void ByteArrayTests::toStringTests()
{
    ByteArray testByteArray("This is a test char array.");

    // See if that, converted to a string, matches the original value.
    QCOMPARE(std::string("This is a test char array."), testByteArray.toString());

    // Clear it out and check the string.
    testByteArray.clear();

    QCOMPARE(std::string(""), testByteArray.toString());
}

void ByteArrayTests::emptyStringTests()
{
    ByteArray testByteArray;

    QCOMPARE(std::string(""), testByteArray.toString());
}

void ByteArrayTests::atTests()
{
    ByteArray testByteArray("This is a test char array.");

    QCOMPARE((size_t)26, testByteArray.size());
    QCOMPARE('T', testByteArray.at(0));
    QCOMPARE('h', testByteArray.at(1));
    QCOMPARE('.', testByteArray.at(25));
    QCOMPARE((char)0x00, testByteArray.at(100));
}

void ByteArrayTests::operatorTests()
{
    ByteArray testByteArray1("This is a test char array.");
    ByteArray testByteArray2;
    ByteArray *testByteArray3;

    // The two objects should NOT match to start with.
    QVERIFY(testByteArray1 != testByteArray2);

    // Copy the data with the = operator.
    testByteArray2 = testByteArray1;

    // They should now match.
    QVERIFY(testByteArray1 == testByteArray2);

    // Clear the 2nd object.
    testByteArray2.clear();

    // The two objects should NOT match again.
    QVERIFY(testByteArray1 != testByteArray2);

    // Populate the 2nd one from a string.
    testByteArray2 = std::string("This is a test char array.");

    // They should match again.
    QVERIFY(testByteArray1 == testByteArray2);

    // Copy the data with the copy ctor.
    testByteArray3 = new ByteArray(testByteArray1);

    // They should now match again.
    QVERIFY(testByteArray1 == (*testByteArray3));

    // Clean up.
    delete testByteArray3;
}

void ByteArrayTests::fromStringTests()
{
    ByteArray testByteArray("This is a test char array.");

    // Change the value using a string.
    testByteArray.fromStdString(std::string("This is a different test char array."));

    // Make sure it matches.
    QCOMPARE(std::string("This is a different test char array."), testByteArray.toString());
}

