#ifndef UICLIPBOARD_H
#define UICLIPBOARD_H

#include <QObject>
#include <QClipboard>

class UiClipboard : public QObject
{
    Q_OBJECT

public:
    explicit UiClipboard(QObject *parent = nullptr);

    Q_INVOKABLE void setText(QString text);

private:
    QClipboard *mClipboard;
};

#endif // UICLIPBOARD_H
