#ifndef QRVIDEORUNNABLE_H
#define QRVIDEORUNNABLE_H

#include <QObject>
#include <QVideoFilterRunnable>

#include <zbar.h>

class QRCodeFilter;

class QRVideoRunnable : public QVideoFilterRunnable
{
public:
    QRVideoRunnable(QRCodeFilter *filter);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
    zbar::Image mImage;
    zbar::ImageScanner mScanner;
    QSize mFrameSize;
    QRCodeFilter *mFilter;
};

#endif // QRVIDEORUNNABLE_H
