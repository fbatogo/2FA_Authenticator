#include "uiclipboard.h"

#include <QGuiApplication>
#include <QClipboard>

UiClipboard::UiClipboard(QObject *parent) :
    QObject(parent)
{
}

void UiClipboard::setText(QString text)
{
    QGuiApplication::clipboard()->setText(text, QClipboard::Clipboard);

    // If this OS supports the "selection" clipboard, copy the data there as well.
    if (QGuiApplication::clipboard()->supportsSelection()) {
        QGuiApplication::clipboard()->setText(text, QClipboard::Selection);
    }
}
