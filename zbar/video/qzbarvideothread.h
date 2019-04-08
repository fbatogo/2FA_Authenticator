#ifndef QZBARVIDEOTHREAD_H
#define QZBARVIDEOTHREAD_H

#include <QThread>

#include "../myqzbarimage.h"
#include <zbar.h>

class QZBarVideoThread : public QThread //, public zbar::Image::Handler
{
    Q_OBJECT

public:
    zbar::Window window;

    QZBarVideoThread();

    void clear();

public slots:
    void requestTermination();

protected:
    void run();

private:
    zbar::Video *mVideo;
    zbar::MyQZBarImage *mImage;
};

#endif // QZBARVIDEOTHREAD_H
