#include "zbarscanthread.h"

#include "logger.h"

ZBarScanThread::ZBarScanThread(QObject *parent) :
    QThread(parent)
{
    mShouldTerminate = false;
}

/**
 * @brief ZBarScanThread::queueFrameToProcess - Queue up the frame that we want our worker
 *      thread to process.
 *
 * @param toProcess - The frame that we want to process with the QR code scanner.
 */
void ZBarScanThread::queueFrameToProcess(const QImage &toProcess)
{
    // Lock the queue.
    mQueueLock.lock();

    // Stuff it in.
    mToProcessQueue.enqueue(toProcess);

    // Then unlock the queue.
    mQueueLock.unlock();
}

/**
 * @brief ZBarScanThread::requestThreadTerminate - Set the flag that indicates that we
 *      want this worker thread to terminate.
 */
void ZBarScanThread::requestThreadTerminate()
{
    LOG_DEBUG("Terminating the thread.");
    mShouldTerminate = true;
}

/**
 * @brief ZBarScanThread::run - The main body of the thread that will process
 *      frames send to it.
 */
void ZBarScanThread::run()
{
    QImage toProcess;

    LOG_DEBUG("Started the frame processing thread.");

    while (!mShouldTerminate) {
        // Get the image out of the queue, if one is available.
        if (mToProcessQueue.size() > 0) {
            // Lock the queue to read out the image.
            mQueueLock.lock();

            // Dequeue the image.
            toProcess = mToProcessQueue.dequeue();

            // Unlock the queue.
            mQueueLock.unlock();

            // XXX Then, convert the image and pass it in to ZBar for processing.
            LOG_DEBUG("Processed image!");
        } else {
            // Nothing to process.  Give up the time slice.
            sleep(0);
        }
    }
}
