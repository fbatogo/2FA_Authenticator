#include "uiclipboard.h"

#include <QGuiApplication>
#include <QClipboard>

UiClipboard::UiClipboard(QObject *parent) :
    QObject(parent)
{
    mClipboard = QGuiApplication::clipboard();
}

void UiClipboard::setText(QString text)
{
    mClipboard->setText(text, QClipboard::Clipboard);
    mClipboard->setText(text, QClipboard::Selection);
}
