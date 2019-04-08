#ifndef QZBARVIDEO_H
#define QZBARVIDEO_H

#include <QtQuick>

#include "qzbarvideothread.h"

class QZBarVideo : public QQuickPaintedItem
{
    Q_OBJECT

public:
    QZBarVideo(QQuickItem *parent = nullptr);
    ~QZBarVideo();

    void paint(QPainter *painter);

signals:
    void terminateThread();

private:
    QZBarVideoThread *mThread;
};

#endif // QZBARVIDEO_H
