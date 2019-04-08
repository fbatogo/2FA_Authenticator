#ifndef QRCODESTRINGPARSERTESTS_H
#define QRCODESTRINGPARSERTESTS_H

#include <QObject>
#include "../zbar/qrcodestringparser.h"

class QRCodeStringParserTests : public QObject
{
    Q_OBJECT

private slots:
    void qrCodeSimpleStringParserTests();
    void qrCodeInvalidStringParserTests();
    void qrCodeFullSetStringParametersTests();
    void qrCodeFullSetStringWithEqualsInValueTests();
    void qrCodeFullSetStringWithAmpTests();
};

//DECLARE_TEST(QRCodeStringParserTests)

#endif // QRCODESTRINGPARSERTESTS_H
