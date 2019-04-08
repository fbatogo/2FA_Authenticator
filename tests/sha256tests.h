#ifndef SHA256TESTS_H
#define SHA256TESTS_H

#include <QObject>

class Sha256Tests : public QObject
{
    Q_OBJECT

private slots:
    void sha256ImplTests();
};

//DECLARE_TEST(Sha256Tests)

#endif // SHA256TESTS_H
