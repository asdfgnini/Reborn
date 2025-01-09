#include "TaoPlainTextEditPrivate.h"

#include "../inc/TaoApplication.h"
#include "../inc/TaoPlainTextEdit.h"
TaoPlainTextEditPrivate::TaoPlainTextEditPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoPlainTextEditPrivate::~TaoPlainTextEditPrivate()
{
}

void TaoPlainTextEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_Q(TaoPlainTextEdit);
    TaoAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<TaoAppBarType::WMMouseActionType>();
    if (actionType == TaoAppBarType::WMLBUTTONDOWN)
    {
        if (!q->toPlainText().isEmpty() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if (actionType == TaoAppBarType::WMLBUTTONUP || actionType == TaoAppBarType::WMNCLBUTTONDOWN)
    {
        if (TaoApplication::containsCursorToItem(q) || (actionType == TaoAppBarType::WMLBUTTONUP && !q->toPlainText().isEmpty()))
        {
            return;
        }
        if (q->hasFocus())
        {
            q->clearFocus();
        }
    }
}

void TaoPlainTextEditPrivate::onThemeChanged(TaoThemeType::ThemeMode themeMode)
{
    Q_Q(TaoPlainTextEdit);
    if (themeMode == TaoThemeType::Light)
    {
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::PlaceholderText, QColor(0x00, 0x00, 0x00, 128));
        q->setPalette(palette);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::PlaceholderText, QColor(0xBA, 0xBA, 0xBA));
        q->setPalette(palette);
    }
}
