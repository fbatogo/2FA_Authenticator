#include "qrvideorunnable.h"

#include "logger.h"

QVideoFrame QRVideoRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
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



    // We don't actually want to change anything, so return what was originally passed in.
    return (*input);
}
