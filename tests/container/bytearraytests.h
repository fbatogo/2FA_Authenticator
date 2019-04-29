#ifndef BYTEARRAYTESTS_H
#define BYTEARRAYTESTS_H

#include <QObject>

class ByteArrayTests : public QObject
{
    Q_OBJECT

private slots:
    void toStringTests();
    void emptyStringTests();
    void atTests();
    void operatorTests();
    void fromStringTests();
};

#endif // BYTEARRAYTESTS_H
