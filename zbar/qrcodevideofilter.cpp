// Code taken from GitHub repo at : https://github.com/mpersano/qml-zbar


#include <cstddef> // zbar needs NULL
#include <zbar.h>

#include "qrcodevideofilter.h"

namespace {

class VideoFilterRunnable : public QVideoFilterRunnable
{
public:
    VideoFilterRunnable(QRCodeVideoFilter *filter);

    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags);

private:
    QSize m_frameSize;
    zbar::Image m_image;
    zbar::ImageScanner m_scanner;
    QRCodeVideoFilter *m_filter;
};

VideoFilterRunnable::VideoFilterRunnable(QRCodeVideoFilter *filter)
    : m_filter(filter)
{
    m_scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);
    m_image.set_format("Y800");
}

QVideoFrame VideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat& surfaceFormat, RunFlags flags)
{
    Q_UNUSED(surfaceFormat);
    Q_UNUSED(flags);

    if (input->handleType() == QAbstractVideoBuffer::NoHandle) {
        if (m_frameSize != input->size()) {
            m_image.set_size(input->width(), input->height());
            m_frameSize = input->size();
        }

        // TODO handle other pixel formats
        if (input->pixelFormat() == QVideoFrame::Format_YUV420P) {
            if (input->map(QAbstractVideoBuffer::ReadOnly)) {
                // XXX simply copy the Y plane and discard UV, is this correct?
                m_image.set_data(input->bits(), input->width()*input->height());
                input->unmap();

                // XXX probably should make this asynchronous
                m_scanner.scan(m_image);

                for (auto it = m_image.symbol_begin(), end = m_image.symbol_end(); it != end; ++it)
                    emit m_filter->detected(QString::fromStdString(it->get_data()));
            }
        }
    }

    return *input;
}

} // anonymous namespace

QRCodeVideoFilter::QRCodeVideoFilter(QObject *parent)
    : QAbstractVideoFilter(parent)
{
}

QVideoFilterRunnable *QRCodeVideoFilter::createFilterRunnable()
{
    return new VideoFilterRunnable(this);
}
