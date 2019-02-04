#include "qrvideorunnable.h"

#include "logger.h"
#ifndef _WIN32
#include <zbar.h>
#endif // _WIN32
#include "qzbarimage.h"

#ifndef _WIN32
QRVideoRunnable::QRVideoRunnable(ZBarScanThread *scanner)
{
    mScanThread = scanner;
}
#else
QRVideoRunnable::QRVideoRunnable()
{

}
#endif // _WIN32

QVideoFrame QRVideoRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    QImage toProcess;

    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    // See if we need to map the buffer in order for it to be usable.
    if (!input->isMapped()) {
        // We need to map the frame in to CPU addressible memory.
        if (!input->map(QAbstractVideoBuffer::ReadOnly)) {
            // Just return the original frame, but indicate we can't
            // read it.
            LOG_ERROR("Unable to map frame!");
            return (*input);
        }
    }

#ifndef _WIN32
    // Queue up the frame for the worker thread to process.
    mScanThread->queueFrameToProcess(QImage(input->bits(), input->width(), input->height(), input->bytesPerLine(), QVideoFrame::imageFormatFromPixelFormat(input->pixelFormat())));
#endif // _WIN32

    // We don't actually want to change anything, so return what was originally passed in.
    return (*input);
}
