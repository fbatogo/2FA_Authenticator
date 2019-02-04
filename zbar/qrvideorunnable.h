#ifndef QRVIDEORUNNABLE_H
#define QRVIDEORUNNABLE_H

#include <QObject>
#include <QVideoFilterRunnable>

#include "zbarscanthread.h"

class QRVideoRunnable : public QVideoFilterRunnable
{
public:
#ifndef _WIN32
    QRVideoRunnable(ZBarScanThread *scanner);
#else
    QRVideoRunnable();
#endif // _WIN32

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
#ifndef _WIN32
    ZBarScanThread *mScanThread;
#endif // _WIN32
};

#endif // QRVIDEORUNNABLE_H
