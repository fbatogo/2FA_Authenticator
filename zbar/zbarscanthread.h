#ifndef ZBARSCANTHREAD_H
#define ZBARSCANTHREAD_H

#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QImage>

class ZBarScanThread : public QThread
{
    Q_OBJECT

public:
    ZBarScanThread(QObject *parent = nullptr);

    void queueFrameToProcess(const QImage &toProcess);

    void requestThreadTerminate();

signals:
    // Signal indicating that a code was found, and if it is
    // in the formamt expected.
    void signalCodeFound(bool expectedFormat, const QString &codeRead);

protected:
    void run();

private:
    QQueue<QImage> mToProcessQueue;
    QMutex mQueueLock;
    bool mShouldTerminate;
};

#endif // ZBARSCANTHREAD_H
