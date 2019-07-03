#ifndef UTILSTESTS_H
#define UTILSTESTS_H

#include <QObject>

class UtilsTests : public QObject
{
    Q_OBJECT

private slots:
    void concatenateFilenameAndPathTests();
    void fileExistsAndDeleteFileTests();
    void copyFileAndDeleteFileTests();
    void moveFileAndDeleteFileTests();
    void createPathAndPathExistsTests();

private:
    bool createTestFile(const QString &filename);
};

#endif // UTILSTESTS_H
