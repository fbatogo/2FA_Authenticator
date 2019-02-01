#ifndef ZBARSCANTHREAD_H
#define ZBARSCANTHREAD_H

#ifndef _WIN32
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QImage>

#include "qzbarimage.h"
#include <zbar.h>

class ZBarScanThread : public QThread
{
    Q_OBJECT

public:
    ZBarScanThread(QObject *parent = nullptr);

    void queueFrameToProcess(const zbar::QZBarImage &toProcess);

    void requestThreadTerminate();

signals:
    // Signal indicating that a code was found, and if it is
    // in the formamt expected.
    void signalCodeFound(const QString &codeRead);

protected:
    void run();

private:
    void emitCode(const QString &foundCode);

    QQueue<zbar::QZBarImage> mToProcessQueue;
    QMutex mQueueLock;
    zbar::ImageScanner mImageScanner;
    bool mShouldTerminate;
};

#endif // _WIN32

#endif // ZBARSCANTHREAD_H
