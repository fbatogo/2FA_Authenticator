#ifndef QRCODESTRINGPARSER_H
#define QRCODESTRINGPARSER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QStringList>

typedef struct {
    QString result;
    QString remainder;
} ResultWithRemainder;

class QRCodeStringParser : public QObject
{
    Q_OBJECT

public:
    QRCodeStringParser(QObject *parent = nullptr);
    QRCodeStringParser(const QString &codeRead, QObject *parent = nullptr);

    Q_INVOKABLE bool isOtpCode() const;

    Q_INVOKABLE QString type() const;
    Q_INVOKABLE QString label() const;
    Q_INVOKABLE QString parametersAsString() const;
    Q_INVOKABLE QString parameterByKey(const QString &key);

protected:
    QMap<QString, QString> mAttributeValues;

private:
    void parseCode(const QString &toParse);
    ResultWithRemainder getType(const QString &schemeStripped);
    ResultWithRemainder getLabel(const QString &typeStripped);
    void parseParameters(const QString &parameters);
    QStringList getAvpStringList(const QString &parameters);
    void splitAndAddAvp(const QString &avpString);

    bool mIsOtpCode;
    QString mType;
    QString mLabel;
    QString mParameterString;
};

#endif // QRCODESTRINGPARSER_H
