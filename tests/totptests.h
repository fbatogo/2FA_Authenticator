#ifndef TOTPTESTS_H
#define TOTPTESTS_H

#include <QObject>

class totpTests : public QObject
{
    Q_OBJECT

private slots:
    void totpInvalidHmacTest();
    void totpTestCase1();
    void totpTestCase2();
    void totpTestCase3();
    void totpTestCase4();
    void totpTestCase5();
    void totpTestCase6();
};

//DECLARE_TEST(totpTests)

#endif // TOTPTESTS_H
