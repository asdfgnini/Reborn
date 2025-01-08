#include "TaoLineEditPrivate.h"

#include "../inc/TaoApplication.h"
#include "../inc/TaoLineEdit.h"

TaoLineEditPrivate::TaoLineEditPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoLineEditPrivate::~TaoLineEditPrivate()
{
}

void TaoLineEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_Q(TaoLineEdit);
    TaoAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<TaoAppBarType::WMMouseActionType>();
    if (actionType == TaoAppBarType::WMLBUTTONDOWN)
    {
        if (q->hasSelectedText() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if (actionType == TaoAppBarType::WMLBUTTONUP || actionType == TaoAppBarType::WMNCLBUTTONDOWN)
    {
        if (TaoApplication::containsCursorToItem(q) || (actionType == TaoAppBarType::WMLBUTTONUP && q->hasSelectedText()))
        {
            return;
        }
        if (q->hasFocus())
        {
            q->clearFocus();
        }
    }
}

void TaoLineEditPrivate::onThemeChanged(TaoThemeType::ThemeMode themeMode)
{
    Q_Q(TaoLineEdit);
    _themeMode = themeMode;
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
