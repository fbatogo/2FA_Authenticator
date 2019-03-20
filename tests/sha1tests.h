#ifndef SHA1TESTS_H
#define SHA1TESTS_H

#include <QObject>
#include <string>

class Sha1Tests : public QObject
{
    Q_OBJECT

private slots:
    void sha1Tests1();
    void sha1Tests2();

private:
    std::string binaryToString(unsigned char *bytes, size_t bytesLength);

};


#endif // SHA1TESTS_H
