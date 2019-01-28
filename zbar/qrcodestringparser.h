#ifndef QRCODESTRINGPARSER_H
#define QRCODESTRINGPARSER_H

#include <QString>

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

    bool mIsOtpCode;
    QString mType;
    QString mLabel;
    QString mParameterString;
    // XXX Add a MAP for the parsed out parameters.
};

#endif // QRCODESTRINGPARSER_H
