#ifndef QRCODESTRINGPARSER_H
#define QRCODESTRINGPARSER_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QMutex>

typedef struct {
    QString result;
    QString remainder;
} ResultWithRemainder;

class QRCodeStringParser : public QObject
{
    Q_OBJECT

public:
    virtual ~QRCodeStringParser();

    static QRCodeStringParser *getInstance();
    static QObject *getQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    void clear();
    bool parseCode(const QString &toParse);

    Q_INVOKABLE bool isOtpCode() const;
    Q_INVOKABLE bool isCodeProcessing() const;
    Q_INVOKABLE void setCodeProcessing(bool newval);

    Q_INVOKABLE QString type() const;
    Q_INVOKABLE QString label() const;
    Q_INVOKABLE QString parametersAsString() const;
    Q_INVOKABLE QString parameterByKey(const QString &key);

protected:
    void updateEngine(QQmlEngine *engine);

    QMap<QString, QString> mAttributeValues;

private:
    QRCodeStringParser(QObject *parent = nullptr);

    ResultWithRemainder getType(const QString &schemeStripped);
    ResultWithRemainder getLabel(const QString &typeStripped);
    void parseParameters(const QString &parameters);
    QStringList getAvpStringList(const QString &parameters);
    void splitAndAddAvp(const QString &avpString);

    bool mIsOtpCode;
    bool mCodeProcessing;
    QString mType;
    QString mLabel;
    QString mParameterString;
    QMutex *mMutex;                  // A mutex to be sure that only one thread is operating on this object at a time.

    QQmlEngine *mQmlEngine;
};

#endif // QRCODESTRINGPARSER_H
