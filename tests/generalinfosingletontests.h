#ifndef GENERALINFOSINGLETONTESTS_H
#define GENERALINFOSINGLETONTESTS_H

#include <QObject>

class GeneralInfoSingletonTests : public QObject
{
    Q_OBJECT

private slots:
    void singletonsTest();
    void versionTest();
    void licenseTest();
    void haveZBarTest();
    void isEncodedProperlyTest();
};

#endif // GENERALINFOSINGLETONTESTS_H
