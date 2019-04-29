#include "bytearraytests.h"

#include "container/bytearray.h"
#include <QtTest>

void ByteArrayTests::toStringTests()
{
    ByteArray testByteArray("This is a test char array.");

    // See if that, converted to a string, matches the original value.
    QCOMPARE("This is a test char array.", testByteArray.toString());
}

void ByteArrayTests::atTests()
{
    ByteArray testByteArray("This is a test char array.");

    QCOMPARE('T', testByteArray.at(0));
    QCOMPARE('h', testByteArray.at(1));

    // XXX FINISH!
}

void ByteArrayTests::operatorTests()
{

}

void ByteArrayTests::fromStringTests()
{

}

void ByteArrayTests::fromCharArrayTests()
{

}
