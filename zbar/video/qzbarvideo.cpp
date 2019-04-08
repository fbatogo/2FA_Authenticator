#include "qzbarvideo.h"

#include <QtX11Extras/QX11Info>

#include <zbar.h>

using namespace zbar;

QZBarVideo::QZBarVideo(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    mThread = new QZBarVideoThread();

    // Attach to the window.
    //mThread->window.attach(QX11Info::display(), window()->winId());

    connect(this, SIGNAL(terminateThread()), mThread, SLOT(requestTermination()));

    mThread->start();
}

QZBarVideo::~QZBarVideo()
{
    // Terminate the running thread.
    emit terminateThread();

    // Wait for it to terminate.
    mThread->wait();

    delete mThread;
}

void QZBarVideo::paint(QPainter *)
{
    if (mThread != nullptr) {
        mThread->window.redraw();
    }
}
