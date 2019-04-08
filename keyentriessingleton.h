#ifndef KEYENTRIESSINGLETON_H
#define KEYENTRIESSINGLETON_H

#include <QObject>
#include <QTimer>
#include <QQmlEngine>
#include <QJSEngine>

#include "keystorage/keyentry.h"
#include "keystorage/keystorage.h"

class KeyEntriesSingleton : public QObject
{
    Q_OBJECT

public:
    ~KeyEntriesSingleton();

    static KeyEntriesSingleton *getInstance();
    static QObject *getQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    void clear();

    bool calculateEntries();

    Q_INVOKABLE bool addKeyEntry(QString identifier, QString secret, int keyType, int otpType, int numberCount, int algorithm, int period, int offset);
    bool addKeyEntry(const KeyEntry &toAdd);

    Q_INVOKABLE bool updateKeyEntry(KeyEntry *currentEntry, QString identifier, QString secret, int keyType, int otpType, int numberCount, int algorithm, int period, int offset);
    bool updateKeyEntry(const KeyEntry &original, const KeyEntry &updated);

    Q_INVOKABLE bool deleteKeyEntry(const QString &toDelete);

    Q_INVOKABLE bool incrementHotpCounter(const QString &identifier);

    Q_INVOKABLE int count();
    Q_INVOKABLE KeyEntry *at(int i);
    Q_INVOKABLE KeyEntry *fromIdentifier(const QString &identifier);

signals:
    void fullRefresh();

private slots:
    void slotUpdateOtpValues();

private:
    explicit KeyEntriesSingleton(QObject *parent = nullptr);

    bool populateEntries();
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

#endif // KEYENTRIESSINGLETON_H
