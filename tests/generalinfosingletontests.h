#ifndef GENERALINFOSINGLETONTESTS_H
#define GENERALINFOSINGLETONTESTS_H

#include <QObject>

class GeneralInfoSingletonTests : public QObject
{
    Q_OBJECT

private slots:
    void versionTest();
    void licenseTest();
    void haveZBarTest();
};

#endif // GENERALINFOSINGLETONTESTS_H
