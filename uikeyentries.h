#ifndef UIKEYENTRIES_H
#define UIKEYENTRIES_H

#include <QObject>

#include "keystorage/keyentry.h"

class UiKeyEntries : public QObject
{
    Q_OBJECT

public:
    explicit UiKeyEntries(QObject *parent = nullptr);
    ~UiKeyEntries();

    bool populateEntries(QList<KeyEntry> &toCopy);

    Q_INVOKABLE int count();
    Q_INVOKABLE KeyEntry *at(int i);

private:
    QList<KeyEntry *> mEntryList;
};

#endif // UIKEYENTRIES_H
