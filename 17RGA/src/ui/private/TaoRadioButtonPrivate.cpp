#include "TaoRadioButtonPrivate.h"

#include "../inc/TaoRadioButton.h"
#include "../inc/TaoTheme.h"
TaoRadioButtonPrivate::TaoRadioButtonPrivate(QObject* parent)
    : QObject(parent)
{
}

TaoRadioButtonPrivate::~TaoRadioButtonPrivate()
{
}

void TaoRadioButtonPrivate::onThemeChanged(TaoThemeType::ThemeMode themeMode)
{
    Q_Q(TaoRadioButton);
    QPalette palette = q->palette();
    palette.setColor(QPalette::WindowText, TaoThemeColor(themeMode, BasicText));
    q->setPalette(palette);
}
