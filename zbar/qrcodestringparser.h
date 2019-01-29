#ifndef QRCODESTRINGPARSER_H
#define QRCODESTRINGPARSER_H

#include <QString>
#include <QMap>
#include <QStringList>

typedef struct {
    QString result;
    QString remainder;
} ResultWithRemainder;

class QRCodeStringParser
{
public:
    QRCodeStringParser(const QString &codeRead);

    bool isOtpCode();

    QString type();
    QString label();
    QString parametersAsString();
    QString parameterByKey(const QString &key);

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

    QMap<QString, QString> mAttributeValues;
};

#endif // QRCODESTRINGPARSER_H
