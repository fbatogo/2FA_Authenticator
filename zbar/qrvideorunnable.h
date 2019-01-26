#ifndef QRVIDEORUNNABLE_H
#define QRVIDEORUNNABLE_H

#include <QObject>
#include <QVideoFilterRunnable>

class QRVideoRunnable : public QVideoFilterRunnable
{
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
};

#endif // QRVIDEORUNNABLE_H
