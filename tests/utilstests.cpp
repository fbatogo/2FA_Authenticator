#include "utilstests.h"

#include <QtTest>

#include <QFile>
#include <QDir>

#include "utils.h"

void UtilsTests::concatenateFilenameAndPathTests()
{
    // If the filename is empty, we should get an empty string.
    QCOMPARE(QString(""), Utils::getInstance()->concatenateFilenameAndPath("/valid/path", ""));

    // Concatenate a root path to a valid filename.
    QCOMPARE(QString("/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("/", "testfilename.fil"));
    QCOMPARE(QString("c:\\testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "testfilename.fil"));
    QCOMPARE(QString("c:/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:/", "testfilename.fil"));

    // Concatenate a root path to a filename with leading slash.
    QCOMPARE(QString("/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("/", "/testfilename.fil"));
    QCOMPARE(QString("c:\\testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "/testfilename.fil"));
    QCOMPARE(QString("c:/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:/", "/testfilename.fil"));
    QCOMPARE(QString("c:\\testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "\\testfilename.fil"));
    QCOMPARE(QString("c:/testfilename.fil"), Utils::getInstance()->concatenateFilenameAndPath("c:/", "\\testfilename.fil"));

    // Attempt to concatenate a filename with a partial path in it.
    QCOMPARE(QString(""), Utils::getInstance()->concatenateFilenameAndPath("/", "/pathportion/testfilename.fil"));
    QCOMPARE(QString(""), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "/pathportion/testfilename.fil"));
    QCOMPARE(QString(""), Utils::getInstance()->concatenateFilenameAndPath("c:/", "/pathportion/testfilename.fil"));
    QCOMPARE(QString(""), Utils::getInstance()->concatenateFilenameAndPath("c:\\", "\\pathportion\\testfilename.fil"));
    QCOMPARE(QString(""), Utils::getInstance()->concatenateFilenameAndPath("c:/", "\\pathportion\\testfilename.fil"));

    // Test a path that we have had problems with.
    QCOMPARE(Utils::getInstance()->concatenateFilenameAndPath("/home/user/.Rollin/", "/keydatabase.db"), QString("/home/user/.Rollin/keydatabase.db"));
}

/**
 * @brief UtilsTests::createTestFile - Create a small text file to use for testing.
 *
 * @param filename - The file name of the text file to create.
 *
 * @return true if the text file was created.  false otherwise.
 */
bool UtilsTests::createTestFile(const QString &filename)
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

void UtilsTests::fileExistsAndDeleteFileTests()
{
    // Check an empty string, should return false.
    QVERIFY(!Utils::getInstance()->fileExists(""));

    // Check a relative path and file, that we know shouldn't exist, with and without the file:/// prefix.
    QVERIFY(!Utils::getInstance()->fileExists("invalid/path"));
    QVERIFY(!Utils::getInstance()->fileExists("file:///invalid/path"));

    // Create a file to check the existance of.
    QVERIFY(createTestFile("testfile.fil"));

    // Check for the created file, with and without the file:/// prefix.
    QVERIFY(Utils::getInstance()->fileExists("testfile.fil"));
    QVERIFY(Utils::getInstance()->fileExists("file:///testfile.fil"));

    // Clean up.
    QVERIFY(Utils::getInstance()->deleteFile("testfile.fil"));
}

void UtilsTests::copyFileAndDeleteFileTests()
{
    // Try invalid versions of the call.
    QVERIFY(!Utils::getInstance()->copyFile("", ""));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    QVERIFY(!Utils::getInstance()->copyFile("testfile.fil", ""));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    QVERIFY(!Utils::getInstance()->copyFile("", "testfile.fil"));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    // Create a test file to copy.
    QVERIFY(createTestFile("testfile.fil"));

    // Make sure our target test file doesn't exist.
    if (Utils::getInstance()->fileExists("othertestfile.fil")) {
        QVERIFY(Utils::getInstance()->deleteFile("othertestfile.fil"));
    }

    // Copy it to a different file name.
    QVERIFY(Utils::getInstance()->copyFile("testfile.fil", "othertestfile.fil"));

    // Make sure no error exists.
    QVERIFY(Utils::getInstance()->lastError().isEmpty());

    // Attempt to copy the file again, should fail because the file already exists.
    QVERIFY(!Utils::getInstance()->copyFile("testfile.fil", "othertestfile.fil"));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    // Clean up the two files.
    QVERIFY(Utils::getInstance()->deleteFile("othertestfile.fil"));

    // Make sure no error exists.
    QVERIFY(Utils::getInstance()->lastError().isEmpty());

    QVERIFY(Utils::getInstance()->deleteFile("testfile.fil"));

    // Make sure no error exists.
    QVERIFY(Utils::getInstance()->lastError().isEmpty());
}

void UtilsTests::moveFileAndDeleteFileTests()
{
    // Try invalid versions of the call.
    QVERIFY(!Utils::getInstance()->moveFile("", ""));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    QVERIFY(!Utils::getInstance()->moveFile("testfile.fil", ""));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    QVERIFY(!Utils::getInstance()->moveFile("", "testfile.fil"));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    // Create a test file to copy.
    QVERIFY(createTestFile("testfile.fil"));

    // Make sure our target test file doesn't exist.
    if (Utils::getInstance()->fileExists("othertestfile.fil")) {
        QVERIFY(Utils::getInstance()->deleteFile("othertestfile.fil"));
    }

    // Move it to a different file name.
    QVERIFY(Utils::getInstance()->moveFile("testfile.fil", "othertestfile.fil"));

    // Make sure no error exists.
    QVERIFY(Utils::getInstance()->lastError().isEmpty());

    // Attempt to move the file again, should fail because the source doesn't exist, and the target does.
    QVERIFY(!Utils::getInstance()->moveFile("testfile.fil", "othertestfile.fil"));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    // Create the source again.
    QVERIFY(createTestFile("testfile.fil"));

    // Try to move the file.  Should fail because the destination already exists.
    QVERIFY(!Utils::getInstance()->moveFile("testfile.fil", "othertestfile.fil"));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    // Clean up the two files.
    QVERIFY(Utils::getInstance()->deleteFile("othertestfile.fil"));

    // Make sure no error exists.
    QVERIFY(Utils::getInstance()->lastError().isEmpty());

    QVERIFY(Utils::getInstance()->deleteFile("testfile.fil"));

    // Make sure no error exists.
    QVERIFY(Utils::getInstance()->lastError().isEmpty());
}

void UtilsTests::createPathAndPathExistsTests()
{
    QDir pathTest;

    // Negative tests.
    QVERIFY(!Utils::getInstance()->pathExists(""));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    QVERIFY(!Utils::getInstance()->createPath(""));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    // Attempt to create a simple path.
    QVERIFY(Utils::getInstance()->createPath("testpath"));

    // Make sure no error exists.
    QVERIFY(Utils::getInstance()->lastError().isEmpty());

    // Make sure it exists.
    QVERIFY(Utils::getInstance()->pathExists("testpath"));

    // Try to create it again, which should fail.
    QVERIFY(!Utils::getInstance()->createPath("testpath"));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    // Remove it.
    QVERIFY(pathTest.remove("testpath"));

    // Make sure it no longer exists.
    QVERIFY(!Utils::getInstance()->pathExists("testpath"));

    // Create a multi-level path.
    QVERIFY(Utils::getInstance()->createPath("testpath/testpathagain"));

    // Make sure no error exists.
    QVERIFY(Utils::getInstance()->lastError().isEmpty());

    // Make sure it exists.
    QVERIFY(Utils::getInstance()->pathExists("testpath/testpathagain"));

    // Try to create it again, which should fail.
    QVERIFY(!Utils::getInstance()->createPath("testpath/testpathagain"));

    // Make sure an error was stored.
    QVERIFY(!Utils::getInstance()->lastError().isEmpty());

    // Remove each component of the path.
    QVERIFY(pathTest.remove("testpath/testpathagain"));
    QVERIFY(pathTest.remove("testpath"));

    // Make sure neither parts of the path exist anymore.
    QVERIFY(!Utils::getInstance()->pathExists("testpath/testpathagain"));
    QVERIFY(!Utils::getInstance()->pathExists("testpath"));
}
