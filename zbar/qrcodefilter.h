#ifndef QRCODEFILTER_H
#define QRCODEFILTER_H

#include <QObject>
#include <QAbstractVideoFilter>

#include "qrcodestringparser.h"
#include "zbarscanthread.h"

class QRCodeFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    QRCodeFilter(QObject *parent = nullptr);
    ~QRCodeFilter();

    QVideoFilterRunnable *createFilterRunnable();

signals:
    void signalFinished(QRCodeStringParser *code);
    void signalBadCodeRead();

private slots:
    void slotCodeFound(const QString &codeRead);

private:
    bool mSlotsConnected;
    ZBarScanThread mScanningThread;
    QVideoFilterRunnable *mFilterRunnable;
};

#endif // QRCODEFILTER_H
