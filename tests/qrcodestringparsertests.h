#ifndef QRCODESTRINGPARSERTESTS_H
#define QRCODESTRINGPARSERTESTS_H

#include <QObject>
#include "../zbar/qrcodestringparser.h"
#include "autotest.h"

class QRCodeStringParserTests : public QObject
{
    Q_OBJECT

private slots:
    void qrCodeSimpleStringParserTests();
    void qrCodeInvalidStringParserTests();
    void qrCodeFullSetStringParametersTests();
};

DECLARE_TEST(QRCodeStringParserTests)

#endif // QRCODESTRINGPARSERTESTS_H
