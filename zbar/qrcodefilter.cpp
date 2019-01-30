#include "qrcodefilter.h"

#include "qrcodestringparser.h"
#include "qrvideorunnable.h"
#include "logger.h"

QRCodeFilter::QRCodeFilter(QObject *parent) :
    QAbstractVideoFilter(parent)
{
    mFilterRunnable = nullptr;
    mSlotsConnected = false;
}

QRCodeFilter::~QRCodeFilter()
{
    // The runnable should be cleaned up by the caller.  But, we need to
    // terminate the worker thread, and disconnect any slots/signals.
    if (mSlotsConnected) {
        LOG_DEBUG("Disconnecting QRCode filter signals/slots.");
        disconnect(&mScanningThread, SIGNAL(signalCodeFound(bool, const QString &)), this, SLOT(slotCodeFound(bool, const QString &)));
    }

    // Terminate the worker thread.
    mScanningThread.requestThreadTerminate();
    mScanningThread.wait();             // Wait for the thread to terminate before moving on.
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
    // If needed, spin up the thread that will process frames.
    if (!mScanningThread.isRunning()) {
        LOG_DEBUG("Starting the image scanning thread...");
        mScanningThread.start();
    }

    // Connect the signal that indicates the code was found with the slot to
    // process it.
    connect(&mScanningThread, SIGNAL(signalCodeFound(bool, const QString &)), this, SLOT(slotCodeFound(bool, const QString &)));
    mSlotsConnected = true;

    return new QRVideoRunnable(&mScanningThread);
}

/**
 * @brief QRCodeFilter::slotCodeFound - Called any time that ZBar finds and reads a
 *      code.  It decides if the code is valid, and if it is, shuts everything down
 *      and sends the code to the main thread to be handled.
 *
 * @param expectedFormat - true if the code read was in the properly format for a
 *      TOTP code.
 * @param codeRead - The actual code that was read from the QR code.
 */
void QRCodeFilter::slotCodeFound(const QString &codeRead)
{
    QRCodeStringParser *parser;

    parser = new QRCodeStringParser(codeRead);

    // Check to see if the code we got is one that we can use.
    if (!parser->isOtpCode()) {
        // Didn't find a valid code.  Emit a "bad code" error, and continue to scan.
        emit signalBadCodeRead();
        return;
    }

    LOG_DEBUG("Found valid QR code!");

    // We found a code.  Terminate the thread, and signal to the UI that we are done.
    mScanningThread.requestThreadTerminate();
    mScanningThread.wait();             // Wait for the thread to terminate before moving on.

    emit signalFinished(parser);
}
