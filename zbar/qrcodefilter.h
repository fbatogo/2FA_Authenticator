#ifndef QRCODEFILTER_H
#define QRCODEFILTER_H

#ifndef _WIN32
#include <QObject>
#include <QAbstractVideoFilter>

#include "qrcodestringparser.h"

class QRCodeFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    QRCodeFilter(QObject *parent = nullptr);
    ~QRCodeFilter();

    QVideoFilterRunnable *createFilterRunnable();

signals:
    void signalFinished(QRCodeStringParser *code);
};

#endif // _WIN32

#endif // QRCODEFILTER_H
