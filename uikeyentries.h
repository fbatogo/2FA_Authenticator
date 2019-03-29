#ifndef UIKEYENTRIES_H
#define UIKEYENTRIES_H

#include <QObject>
#include <QTimer>

#include "keystorage/keyentry.h"
#include "keystorage/keystorage.h"

class UiKeyEntries : public QObject
{
    Q_OBJECT

public:
    explicit UiKeyEntries(QObject *parent = nullptr);
    ~UiKeyEntries();

    void clear();

    bool populateEntries();
    bool calculateEntries();

    bool addKeyEntry(const KeyEntry &toAdd);
    bool updateKeyEntry(const KeyEntry &original, const KeyEntry &updated);
    bool deleteKeyEntry(const QString &toDelete);

    Q_INVOKABLE int count();
    Q_INVOKABLE KeyEntry *at(int i);
    Q_INVOKABLE KeyEntry *fromIdentifier(const QString &identifier);

signals:
    void fullRefresh();

private slots:
    void slotUpdateOtpValues();

private:
    int indexFromIdentifierInMemory(const QString &identifier);
    KeyEntry *fromIdentifierInMemory(const QString &identifier);
    KeyEntry *fromIdentifierInKeyStorage(const QString &identifier);
    bool deleteKeyEntryFromMemory(const QString &toDelete);
    bool updateKeyEntryInMemory(const KeyEntry &original, const KeyEntry &updated);
    bool addKeyEntryInMemory(const KeyEntry &toAdd);

    bool updateTimer();
    int shortestUpdatePeriod();

    QList<KeyEntry *> mEntryList;

    QTimer mUpdateTimer;

    KeyStorage mKeyStorage;
};

#endif // UIKEYENTRIES_H
