#include "qrcodefilter.h"

#ifndef NO_ZBAR
#include "qrcodestringparser.h"
#include "qrvideorunnable.h"
#include "logger.h"
#include <iostream>

QRCodeFilter::QRCodeFilter(QObject *parent) :
    QAbstractVideoFilter(parent)
{
}

QRCodeFilter::~QRCodeFilter()
{
}

/**
 * @brief QRCodeFilter::createFilterRunnable - Create the runnable that will process
 *      the frames that are read from the camera.
 *
 * @return QVideoFilterRunnable pointer for the runnable that will scan the frames
 *      passed in.
 */
QVideoFilterRunnable *QRCodeFilter::createFilterRunnable()
{
    return new QRVideoRunnable(this);
}

#endif // NO_ZBAR
