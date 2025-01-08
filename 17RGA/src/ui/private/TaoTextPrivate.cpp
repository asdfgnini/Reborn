#include "TaoTextPrivate.h"

#include "../inc/TaoText.h"

TaoTextPrivate::TaoTextPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoTextPrivate::~TaoTextPrivate()
{
}

void TaoTextPrivate::onThemeChanged(TaoThemeType::ThemeMode themeMode)
{
    Q_Q(TaoText);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    if (themeMode == TaoThemeType::Light)
    {
        palette.setColor(QPalette::WindowText, Qt::black);
        q->setPalette(palette);
    }
    else
    {
        palette.setColor(QPalette::WindowText, Qt::white);
        q->setPalette(palette);
    }
}
