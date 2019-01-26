#ifndef QRCODEFILTER_H
#define QRCODEFILTER_H

#include <QObject>
#include <QAbstractVideoFilter>

class QRCodeFilter : public QAbstractVideoFilter
{
public:
    QRCodeFilter(QObject *parent = nullptr);

    QVideoFilterRunnable *createFilterRunnable();

signals:
    void finished(QObject *result);
};

#endif // QRCODEFILTER_H
