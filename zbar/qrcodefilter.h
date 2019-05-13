#ifndef QRCODEFILTER_H
#define QRCODEFILTER_H

#ifndef NO_ZBAR
#include <QObject>
#include <QAbstractVideoFilter>

#include "qrcodestringparser.h"

class QRCodeFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    explicit QRCodeFilter(QObject *parent = nullptr);
    ~QRCodeFilter();

    QVideoFilterRunnable *createFilterRunnable();

signals:                    //NOSONAR
    void signalFinished();
};

#endif // NO_ZBAR

#endif // QRCODEFILTER_H
