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
    void setAtTests();
    void equalityTests();
    void fromOthersTests();
    void assignmentTests();
    void appendTests();
    void expandedBufferAppendTests();
    void truncateTests();
};

#endif // BYTEARRAYTESTS_H
