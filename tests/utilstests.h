#ifndef UTILSTESTS_H
#define UTILSTESTS_H

#include <QObject>

class UtilsTests : public QObject
{
    Q_OBJECT

private slots:
    void concatenateFilenameAndPathTests();
};

#endif // UTILSTESTS_H
