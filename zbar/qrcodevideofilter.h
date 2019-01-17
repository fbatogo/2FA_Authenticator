// Original code taken from GitHub repo at : https://github.com/mpersano/qml-zbar

#ifndef QRCODEVIDEOFILTER_H
#define QRCODEVIDEOFILTER_H

#pragma once

#include <QQuickView>
#include <QAbstractVideoFilter>

class QRCodeVideoFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    QRCodeVideoFilter(QObject *parent = nullptr);

    QVideoFilterRunnable *createFilterRunnable() override;

signals:
    void detected(const QString& code);
};


#endif // QRCODEVIDEOFILTER_H
