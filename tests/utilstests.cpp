#include "utilstests.h"

#include <QtTest>

#include "utils.h"

void UtilsTests::concatenateFilenameAndPathTests()
{
    // If the filename is empty, we should get an empty string.
    QCOMPARE(QString(""), Utils::concatenateFilenameAndPath("/valid/path", ""));

    // Concatenate a root path to a valid filename.
    QCOMPARE(QString("/testfilename.fil"), Utils::concatenateFilenameAndPath("/", "testfilename.fil"));
    QCOMPARE(QString("c:\\testfilename.fil"), Utils::concatenateFilenameAndPath("c:\\", "testfilename.fil"));
    QCOMPARE(QString("c:/testfilename.fil"), Utils::concatenateFilenameAndPath("c:/", "testfilename.fil"));

    // Concatenate a root path to a filename with leading slash.
    QCOMPARE(QString("/testfilename.fil"), Utils::concatenateFilenameAndPath("/", "/testfilename.fil"));
    QCOMPARE(QString("c:\\testfilename.fil"), Utils::concatenateFilenameAndPath("c:\\", "/testfilename.fil"));
    QCOMPARE(QString("c:/testfilename.fil"), Utils::concatenateFilenameAndPath("c:/", "/testfilename.fil"));
    QCOMPARE(QString("c:\\testfilename.fil"), Utils::concatenateFilenameAndPath("c:\\", "\\testfilename.fil"));
    QCOMPARE(QString("c:/testfilename.fil"), Utils::concatenateFilenameAndPath("c:/", "\\testfilename.fil"));

    // Attempt to concatenate a filename with a partial path in it.
    QCOMPARE(QString(""), Utils::concatenateFilenameAndPath("/", "/pathportion/testfilename.fil"));
    QCOMPARE(QString(""), Utils::concatenateFilenameAndPath("c:\\", "/pathportion/testfilename.fil"));
    QCOMPARE(QString(""), Utils::concatenateFilenameAndPath("c:/", "/pathportion/testfilename.fil"));
    QCOMPARE(QString(""), Utils::concatenateFilenameAndPath("c:\\", "\\pathportion\\testfilename.fil"));
    QCOMPARE(QString(""), Utils::concatenateFilenameAndPath("c:/", "\\pathportion\\testfilename.fil"));

    // Test a path that we have had problems with.
    QCOMPARE(Utils::concatenateFilenameAndPath("/home/user/.Rollin/", "/keydatabase.db"), QString("/home/user/.Rollin/keydatabase.db"));
}
