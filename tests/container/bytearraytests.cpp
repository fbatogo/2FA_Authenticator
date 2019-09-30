#include <testsuitebase.h>

#include "container/bytearray.h"

SIMPLE_TEST_SUITE(ByteArrayTests, ByteArray);

TEST_F(ByteArrayTests, ToStringTests)
{
    ByteArray testByteArray("This is a test char array.");

    // See if that, converted to a string, matches the original value.
    EXPECT_EQ(std::string("This is a test char array."), testByteArray.toString());

    // Clear it out and check the string.
    testByteArray.clear();

    EXPECT_EQ(std::string(""), testByteArray.toString());
}

TEST_F(ByteArrayTests, EmptyStringTests)
{
    ByteArray testByteArray;

    EXPECT_EQ(std::string(""), testByteArray.toString());
}

TEST_F(ByteArrayTests, AtTests)
{
    ByteArray testByteArray("This is a test char array.");

    EXPECT_EQ((size_t)26, testByteArray.size());
    EXPECT_EQ((unsigned char)'T', testByteArray.at(0));
    EXPECT_EQ((unsigned char)'h', testByteArray.at(1));
    EXPECT_EQ((unsigned char)'.', testByteArray.at(25));
    EXPECT_EQ((unsigned char)0x00, testByteArray.at(100));
}

TEST_F(ByteArrayTests, SetAtTests)
{
    ByteArray testArray(10, false);

    // Try to set a byte when there is no data.
    EXPECT_TRUE(!testArray.setAt(10, 'A'));

    testArray.fromStdString("Test String");

    // Change the first character.
    EXPECT_TRUE(testArray.setAt(0, 'A'));

    EXPECT_EQ(std::string("Aest String"), testArray.toString());
}

TEST_F(ByteArrayTests, EqualityTests)
{
    ByteArray testByteArray1("This is a test char array.");
    ByteArray testByteArray2;
    ByteArray *testByteArray3;

    // The two objects should NOT match to start with.
    EXPECT_TRUE(testByteArray1 != testByteArray2);

    // Copy the data with the = operator.
    testByteArray2 = testByteArray1;

    // They should now match.
    EXPECT_TRUE(testByteArray1 == testByteArray2);

    // Clear the 2nd object.
    testByteArray2.clear();

    // The two objects should NOT match again.
    EXPECT_TRUE(testByteArray1 != testByteArray2);

    // Populate the 2nd one from a string.
    testByteArray2 = std::string("This is a test char array.");

    // They should match again.
    EXPECT_TRUE(testByteArray1 == testByteArray2);

    // Same length string, but slightly different text.
    testByteArray2 = std::string("This is a test char array!");

    // They should note match.
    EXPECT_TRUE(testByteArray1 != testByteArray2);

    // Copy the data with the copy ctor.
    testByteArray3 = new ByteArray(testByteArray1);

    // They should now match again.
    EXPECT_TRUE(testByteArray1 == (*testByteArray3));

    // Clean up.
    delete testByteArray3;
}

TEST_F(ByteArrayTests, FromOthersTests)
{
    ByteArray testByteArray(std::string("This is a test char array."));
    char *cString;

    // Change the value using a string.
    testByteArray.fromStdString(std::string("This is a different test char array."));

    // Make sure it matches.
    EXPECT_EQ(std::string("This is a different test char array."), testByteArray.toString());

    // Create a C style string to  assign to the byte array.
    cString = strdup("This is a test C style string!");
    ASSERT_TRUE(nullptr != cString);

    // Update testByteArray with the C string.
    testByteArray.fromCharArray(cString);

    // Make sure testByteArray has the data we expect.
    EXPECT_EQ(std::string("This is a test C style string!"), testByteArray.toString());

    // Clear out the testByteArray.
    testByteArray.clear();

    EXPECT_EQ(std::string(""), testByteArray.toString());

    // Then, add the string back in, this time as an unsigned char array.
    testByteArray.fromUCharArray(reinterpret_cast<unsigned char *>(cString));

    // And make sure it is what we expect.
    EXPECT_EQ(std::string("This is a test C style string!"), testByteArray.toString());

    // Then, clean up the memory we used.
    free(cString);
}

TEST_F(ByteArrayTests, AssignmentTests)
{
    ByteArray ba1;
    ByteArray ba2;

    // ba1 should be empty and null.
    EXPECT_EQ(std::string(""), ba1.toString());
    EXPECT_EQ(nullptr, ba1.toCharArrayPtr());
    EXPECT_EQ(nullptr, ba1.toUCharArrayPtr());

    // Assign a string to it.
    ba1 = std::string("This is a test string!");

    // Make sure it is what we expect.
    EXPECT_EQ(std::string("This is a test string!"), ba1.toString());

    // ba2 should be empty.
    EXPECT_EQ(std::string(""), ba2.toString());
    EXPECT_EQ(nullptr, ba2.toCharArrayPtr());
    EXPECT_EQ(nullptr, ba2.toUCharArrayPtr());

    // Assign the data in ba1 to ba2.
    ba2 = ba1;

    // Make sure it is what we expect.
    EXPECT_EQ(std::string("This is a test string!"), ba2.toString());

    // Clear out ba1 and then check again, to be sure that the data was copied, and
    // not just a pointer to the data was copied.
    ba1.clear();

    // ba1 should be empty now..
    EXPECT_EQ(std::string(""), ba1.toString());
    EXPECT_EQ(nullptr, ba1.toCharArrayPtr());
    EXPECT_EQ(nullptr, ba1.toUCharArrayPtr());

    // But, ba2 should still have our data.
    EXPECT_EQ(std::string("This is a test string!"), ba2.toString());
}

TEST_F(ByteArrayTests, AppendTests)
{
    ByteArray testByteArray("This is some initial text.");
    ByteArray toAppendByteArray("This is text to append!");
    char *cString;

    // Tell the object to zero the array when freeing memory.
    testByteArray.setZeroOnFree(true);

    // Try to append a null C string.
    EXPECT_TRUE(testByteArray.append((char *)nullptr, 100));

    // Append an std::string to the end.
    EXPECT_TRUE(testByteArray.append(std::string(" So is this!")));

    // Make sure we got the appended data back.
    EXPECT_EQ(std::string("This is some initial text. So is this!"), testByteArray.toString());

    // Clear the string, and append to an empty object.
    testByteArray.clear();
    EXPECT_TRUE(testByteArray.append(std::string("So is this!")));
    EXPECT_EQ(std::string("So is this!"), testByteArray.toString());

    // Allocate a C style string to append.
    cString = strdup("C style string!");

    EXPECT_TRUE(testByteArray.append(cString));
    EXPECT_EQ(std::string("So is this!C style string!"), testByteArray.toString());

    // Append it as an unsigned char array.
    EXPECT_TRUE(testByteArray.append(reinterpret_cast<unsigned char *>(cString)));
    EXPECT_EQ(std::string("So is this!C style string!C style string!"), testByteArray.toString());

    // Clean up.
    free(cString);
    cString = nullptr;

    // Append a single character.
    EXPECT_TRUE(testByteArray.append('A'));
    EXPECT_EQ(std::string("So is this!C style string!C style string!A"), testByteArray.toString());

    // Append another ByteArray object.
    EXPECT_TRUE(testByteArray.append(toAppendByteArray));
    EXPECT_EQ(std::string("So is this!C style string!C style string!AThis is text to append!"), testByteArray.toString());
}

TEST_F(ByteArrayTests, ExpandedBufferAppendTests)
{
    ByteArray testByteArray("This is some initial text.");
    char *cString;

    // Tell the object to allocate a bunch of extra space. (On the next allocation.)
    testByteArray.setExtraAllocation(512);

    // Append an std::string to the end.  (This should cause a new allocation, with the extra space.)
    EXPECT_TRUE(testByteArray.append(std::string(" So is this!")));

    // Make sure we got the appended data back.
    EXPECT_EQ(std::string("This is some initial text. So is this!"), testByteArray.toString());

    // Clear the string, and append to an empty object.  (This should also cause a new allocation, because we do a clear().)
    testByteArray.clear();
    EXPECT_TRUE(testByteArray.append(std::string("So is this!")));
    EXPECT_EQ(std::string("So is this!"), testByteArray.toString());

    // Allocate a C style string to append.
    cString = strdup("C style string!");

    // Append data to the existing byte array.  (This should only cause a copy, since the buffer should be large enough to hold it all.)
    EXPECT_TRUE(testByteArray.append(cString));
    EXPECT_EQ(std::string("So is this!C style string!"), testByteArray.toString());

    // Append it as an unsigned char array.  (This should also only cause a copy.)
    EXPECT_TRUE(testByteArray.append(reinterpret_cast<unsigned char *>(cString)));
    EXPECT_EQ(std::string("So is this!C style string!C style string!"), testByteArray.toString());

    // Append a single character.  (This should also only cause a copy.)
    EXPECT_TRUE(testByteArray.append('A'));
    EXPECT_EQ(std::string("So is this!C style string!C style string!A"), testByteArray.toString());

    // Clean up.
    free(cString);
}

TEST_F(ByteArrayTests, TruncateTests)
{
    ByteArray testByteArray("This is some initial text.");

    // Try to truncate beyond the length of the data.
    EXPECT_TRUE(!testByteArray.truncate(1000));

    // Try to truncate the exact length of the data.
    EXPECT_TRUE(testByteArray.truncate(testByteArray.size()));

    // Make sure the data is still what we expect.
    EXPECT_EQ(std::string("This is some initial text."), testByteArray.toString());

    // Chop the period off.
    EXPECT_TRUE(testByteArray.truncate(testByteArray.size()-1));

    // Make sure the data is what we expect.
    EXPECT_EQ(std::string("This is some initial text"), testByteArray.toString());

    // Chop it down to 4 characters.
    EXPECT_TRUE(testByteArray.truncate(4));

    // Verify it.
    EXPECT_EQ((size_t)4, testByteArray.size());
    EXPECT_EQ(std::string("This"), testByteArray.toString());

    // Chop it down to nothing, which should be the same as calling clear().
    EXPECT_TRUE(testByteArray.truncate(0));

    // Verify it.
    EXPECT_EQ((size_t)0, testByteArray.size());
    EXPECT_EQ(std::string(""), testByteArray.toString());
}
