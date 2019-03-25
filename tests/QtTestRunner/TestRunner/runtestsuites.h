#ifndef RUNTESTSUITES_H
#define RUNTESTSUITES_H

#include <QString>
#include <QStringList>
#include <QObject>

class RunTestSuites
{
public:
    RunTestSuites();

    void setLogPrefix(const QString &prefix);
    void createLogFiles(QString fileType);

    void setLogToConsole(bool shouldLogToConsole);

    void setVerbosity(int verbosity);

    void doNotReportFailedTestsInReturnCode();

    void addTest(QObject *testObject);

    int executeAll();
//    int executeByPattern(QString pattern);

protected:
    QStringList buildArgumentListForObject(QObject *toTest);

private:
    QString mLogPrefix;
    QString mLogFileType;
    bool mReportFailedTestsInReturnCode;
    int mVerbosity;
    bool mLogToConsole;

    std::vector<QObject*> mTestObjects;
};

#endif // RUNTESTSUITES_H
