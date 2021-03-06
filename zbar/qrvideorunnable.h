#ifndef QRVIDEORUNNABLE_H
#define QRVIDEORUNNABLE_H

#ifndef NO_ZBAR

#include <QObject>
#include <QVideoFilterRunnable>

#include <zbar.h>

class QRCodeFilter;

class QRVideoRunnable : public QVideoFilterRunnable
{
public:
    explicit QRVideoRunnable(QRCodeFilter *filter);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
    zbar::Image mImage;
    zbar::ImageScanner mScanner;
    QSize mFrameSize;
    QRCodeFilter *mFilter;
};

#endif
#endif // QRVIDEORUNNABLE_H
