#ifndef NO_ZBAR

#include "qrvideorunnable.h"

#include "logger.h"
#include <zbar.h>
#include <iostream>
#include "qrcodefilter.h"
#include "qrcodestringparser.h"

QRVideoRunnable::QRVideoRunnable(QRCodeFilter *filter) :
    mFilter(filter)
{
    mScanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);
    mImage.set_format("Y800");
}

QVideoFrame QRVideoRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    // If our QRCodeStringParser singleton indicates that we have a code in processing, then don't process this frame.
    if ((!QRCodeStringParser::getInstance()->isCodeProcessing()) &&
            (input->handleType() == QAbstractVideoBuffer::NoHandle)) {

        if (mFrameSize != input->size()) {
            mImage.set_size(input->width(), input->height());
            mFrameSize = input->size();
        }

        if ((input->pixelFormat() == QVideoFrame::Format_YUV420P) &&
                (input->map(QAbstractVideoBuffer::ReadOnly))) {
            mImage.set_data(input->bits(), input->width()*input->height());
            input->unmap();

            mScanner.scan(mImage);

            for (auto it = mImage.symbol_begin(), end = mImage.symbol_end(); it != end; ++it) {
                // Feed the text to the QRCodeStringParser singleton.
                QRCodeStringParser::getInstance()->parseCode(QString::fromStdString(it->get_data()));

                // The QR code appears to be a valid TOTP code.
                emit mFilter->signalFinished();
            }
        }
    }

    return *input;
}

#endif // NO_ZBAR
