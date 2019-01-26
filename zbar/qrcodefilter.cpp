#include "qrcodefilter.h"

#include "qrvideorunnable.h"

QRCodeFilter::QRCodeFilter(QObject *parent) :
    QAbstractVideoFilter(parent)
{

}

QVideoFilterRunnable *QRCodeFilter::createFilterRunnable()
{
    return new QRVideoRunnable();
}
