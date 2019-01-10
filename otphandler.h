#ifndef OTPHANDLER_H
#define OTPHANDLER_H

#include <QObject>

class OtpHandler : public QObject
{
    Q_OBJECT

public:
    OtpHandler();

    Q_INVOKABLE QStringList otpSiteList();
    Q_INVOKABLE QString otpCodeFor(QString siteId);
};

#endif // OTPHANDLER_H
