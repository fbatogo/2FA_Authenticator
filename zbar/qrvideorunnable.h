#ifndef QRVIDEORUNNABLE_H
#define QRVIDEORUNNABLE_H

#include <QObject>
#include <QVideoFilterRunnable>

#include "zbarscanthread.h"

class QRVideoRunnable : public QVideoFilterRunnable
{
public:
    QRVideoRunnable(ZBarScanThread *scanner);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
    ZBarScanThread *mScanThread;
};

#endif // QRVIDEORUNNABLE_H
