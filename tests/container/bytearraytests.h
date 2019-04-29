#ifndef BYTEARRAYTESTS_H
#define BYTEARRAYTESTS_H

#include <QObject>

class ByteArrayTests : public QObject
{
    Q_OBJECT

private slots:
    void toStringTests();
    void atTests();
    void operatorTests();
    void fromStringTests();
    void fromCharArrayTests();
};

#endif // BYTEARRAYTESTS_H
