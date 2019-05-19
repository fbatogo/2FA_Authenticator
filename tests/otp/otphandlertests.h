#ifndef OTPHANDLERTESTS_H
#define OTPHANDLERTESTS_H

#include <QObject>

class OtpHandlerTests : public QObject
{
    Q_OBJECT

private slots:
    void calculateHotpForKeyEntry();
    void calculateTotpForKeyEntry();
};

#endif // OTPHANDLERTESTS_H
