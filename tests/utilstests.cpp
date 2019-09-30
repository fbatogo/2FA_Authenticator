#include <testsuitebase.h>

#include <QFile>
#include <QDir>

#include "utils.h"

EMPTY_TEST_SUITE(UtilsTests);

bool createTestFile(const QString &filename);


TEST_F(UtilsTests, ConcatenateFilenameAndPathTests)
{
    // If the filename is empty, we should get an empty string.
    EXPECT_EQ(QString(""), Utils::getInstance()->concatenateFilenameAndPath("/valid/path", ""));

    // Concatenate a root path to a valid filename.
    EXPECT_EQ(QString("/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("/", "testfilename.fil"));
    EXPECT_EQ(QString("c:\\testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "testfilename.fil"));
    EXPECT_EQ(QString("c:/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:/", "testfilename.fil"));

    // Concatenate a root path to a filename with leading slash.
    EXPECT_EQ(QString("/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("/", "/testfilename.fil"));
    EXPECT_EQ(QString("c:\\testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "/testfilename.fil"));
    EXPECT_EQ(QString("c:/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:/", "/testfilename.fil"));
    EXPECT_EQ(QString("c:\\testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "\\testfilename.fil"));
    EXPECT_EQ(QString("c:/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:/", "\\testfilename.fil"));

    // Attempt to concatenate a filename with a partial path in it.
    EXPECT_EQ(QString(""), Utils::getInstance()->concatenateFilenameAndPath("/", "/pathportion/testfilename.fil"));
    EXPECT_EQ(QString(""), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "/pathportion/testfilename.fil"));
    EXPECT_EQ(QString(""), Utils::getInstance()->concatenateFilenameAndPath("c:/", "/pathportion/testfilename.fil"));
    EXPECT_EQ(QString(""), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "\\pathportion\\testfilename.fil"));
    EXPECT_EQ(QString(""), Utils::getInstance()->concatenateFilenameAndPath("c:/", "\\pathportion\\testfilename.fil"));

    // Test a path that we have had problems with.
    EXPECT_EQ(Utils::getInstance()->concatenateFilenameAndPath("/home/user/.Rollin/", "/keydatabase.db"), QString("/home/user/.Rollin/keydatabase.db"));
}


TEST_F(UtilsTests, FileExistsAndDeleteFileTests)
{
    // Check an empty string, should return false.
    EXPECT_TRUE(!Utils::getInstance()->fileExists(""));

    // Check a relative path and file, that we know shouldn't exist, with and without the file:/// prefix.
    EXPECT_TRUE(!Utils::getInstance()->fileExists("invalid/path"));
    EXPECT_TRUE(!Utils::getInstance()->fileExists("file:///invalid/path"));

    // Create a file to check the existance of.
    EXPECT_TRUE(createTestFile("testfile.fil"));

    // Check for the created file, with and without the file:/// prefix.
    EXPECT_TRUE(Utils::getInstance()->fileExists("testfile.fil"));
    EXPECT_TRUE(Utils::getInstance()->fileExists("file:///testfile.fil"));

    // Clean up.
    EXPECT_TRUE(Utils::getInstance()->deleteFile("testfile.fil"));
}

TEST_F(UtilsTests, CopyFileAndDeleteFileTests)
{
    // Try invalid versions of the call.
    EXPECT_TRUE(!Utils::getInstance()->copyFile("", ""));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    EXPECT_TRUE(!Utils::getInstance()->copyFile("testfile.fil", ""));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    EXPECT_TRUE(!Utils::getInstance()->copyFile("", "testfile.fil"));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    // Create a test file to copy.
    EXPECT_TRUE(createTestFile("testfile.fil"));

    // Make sure our target test file doesn't exist.
    if (Utils::getInstance()->fileExists("othertestfile.fil")) {
        EXPECT_TRUE(Utils::getInstance()->deleteFile("othertestfile.fil"));
    }

    // Copy it to a different file name.
    EXPECT_TRUE(Utils::getInstance()->copyFile("testfile.fil", "othertestfile.fil"));

    // Make sure no error exists.
    EXPECT_TRUE(Utils::getInstance()->lastError().isEmpty());

    // Attempt to copy the file again, should fail because the file already exists.
    EXPECT_TRUE(!Utils::getInstance()->copyFile("testfile.fil", "othertestfile.fil"));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    // Clean up the two files.
    EXPECT_TRUE(Utils::getInstance()->deleteFile("othertestfile.fil"));

    // Make sure no error exists.
    EXPECT_TRUE(Utils::getInstance()->lastError().isEmpty());

    EXPECT_TRUE(Utils::getInstance()->deleteFile("testfile.fil"));

    // Make sure no error exists.
    EXPECT_TRUE(Utils::getInstance()->lastError().isEmpty());
}

TEST_F(UtilsTests, MoveFileAndDeleteFileTests)
{
    // Try invalid versions of the call.
    EXPECT_TRUE(!Utils::getInstance()->moveFile("", ""));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    EXPECT_TRUE(!Utils::getInstance()->moveFile("testfile.fil", ""));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    EXPECT_TRUE(!Utils::getInstance()->moveFile("", "testfile.fil"));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    // Create a test file to copy.
    EXPECT_TRUE(createTestFile("testfile.fil"));

    // Make sure our target test file doesn't exist.
    if (Utils::getInstance()->fileExists("othertestfile.fil")) {
        EXPECT_TRUE(Utils::getInstance()->deleteFile("othertestfile.fil"));
    }

    // Move it to a different file name.
    EXPECT_TRUE(Utils::getInstance()->moveFile("testfile.fil", "othertestfile.fil"));

    // Make sure no error exists.
    EXPECT_TRUE(Utils::getInstance()->lastError().isEmpty());

    // Attempt to move the file again, should fail because the source doesn't exist, and the target does.
    EXPECT_TRUE(!Utils::getInstance()->moveFile("testfile.fil", "othertestfile.fil"));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    // Create the source again.
    EXPECT_TRUE(createTestFile("testfile.fil"));

    // Try to move the file.  Should fail because the destination already exists.
    EXPECT_TRUE(!Utils::getInstance()->moveFile("testfile.fil", "othertestfile.fil"));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    // Clean up the two files.
    EXPECT_TRUE(Utils::getInstance()->deleteFile("othertestfile.fil"));

    // Make sure no error exists.
    EXPECT_TRUE(Utils::getInstance()->lastError().isEmpty());

    EXPECT_TRUE(Utils::getInstance()->deleteFile("testfile.fil"));

    // Make sure no error exists.
    EXPECT_TRUE(Utils::getInstance()->lastError().isEmpty());
}

TEST_F(UtilsTests, CreatePathAndPathExistsTests)
{
    QDir pathTest;

    // Negative tests.
    EXPECT_TRUE(!Utils::getInstance()->pathExists(""));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    EXPECT_TRUE(!Utils::getInstance()->createPath(""));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    // If our test path already exists, clean it up.
    if (Utils::getInstance()->pathExists("testpath")) {
        pathTest.setPath("testpath");
        EXPECT_TRUE(pathTest.removeRecursively());
    }

    // Attempt to create a simple path.
    EXPECT_TRUE(Utils::getInstance()->createPath("testpath"));

    // Make sure no error exists.
    EXPECT_TRUE(Utils::getInstance()->lastError().isEmpty());

    // Make sure it exists.
    EXPECT_TRUE(Utils::getInstance()->pathExists("testpath"));

    // Try to create it again, which should fail.
    EXPECT_TRUE(!Utils::getInstance()->createPath("testpath"));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    // Remove it.
    pathTest.setPath("testpath");
    EXPECT_TRUE(pathTest.removeRecursively());

    // Make sure it no longer exists.
    EXPECT_TRUE(!Utils::getInstance()->pathExists("testpath"));

    // Create a multi-level path.
    EXPECT_TRUE(Utils::getInstance()->createPath("testpath/testpathagain"));

    // Make sure no error exists.
    EXPECT_TRUE(Utils::getInstance()->lastError().isEmpty());

    // Make sure it exists.
    EXPECT_TRUE(Utils::getInstance()->pathExists("testpath/testpathagain"));

    // Try to create it again, which should fail.
    EXPECT_TRUE(!Utils::getInstance()->createPath("testpath/testpathagain"));

    // Make sure an error was stored.
    EXPECT_TRUE(!Utils::getInstance()->lastError().isEmpty());

    // Remove each component of the path.
    pathTest.setPath("testpath");
    EXPECT_TRUE(pathTest.removeRecursively());

    // Make sure neither parts of the path exist anymore.
    EXPECT_TRUE(!Utils::getInstance()->pathExists("testpath/testpathagain"));
    EXPECT_TRUE(!Utils::getInstance()->pathExists("testpath"));
}

/**
 * @brief createTestFile - Create a small text file to use for testing.
 *
 * @param filename - The file name of the text file to create.
 *
 * @return true if the text file was created.  false otherwise.
 */
bool createTestFile(const QString &filename)
{
    FILE *fp;

    fp = fopen(filename.toStdString().c_str(), "w");
    if (nullptr == fp) {
        return false;
    }

    // Write a string to the file.
    fprintf(fp, "This is a test file.  If found, please delete!");

    // Close the file.
    fclose(fp);

    return true;
}
