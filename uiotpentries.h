#ifndef UIOTPENTRIES_H
#define UIOTPENTRIES_H

#include <QObject>

#include "otp/otpentry.h"
#include "keystorage/keyentry.h"

class UiOtpEntries : public QObject
{
    Q_OBJECT

public:
    explicit UiOtpEntries(QObject *parent = nullptr);
    ~UiOtpEntries();

    bool populateEntries(QList<KeyEntry> &toCalculate);

    Q_INVOKABLE int count();
    Q_INVOKABLE OtpEntry *at(int i);

private:
    OtpEntry *calculateEntry(const KeyEntry &calc);

    QList<OtpEntry *> mEntryList;
};

#endif // UIOTPENTRIES_H
