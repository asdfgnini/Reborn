#include "TaoDockWidgetPrivate.h"

#include <QLinearGradient>

#include "../inc/TaoDockWidget.h"
#include "../inc/TaoDockWidgetTitleBar.h"

TaoDockWidgetPrivate::TaoDockWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoDockWidgetPrivate::~TaoDockWidgetPrivate()
{
}

void TaoDockWidgetPrivate::onThemeModeChanged(TaoThemeType::ThemeMode themeMode)
{
    Q_Q(TaoDockWidget);
    q->update();
    _themeMode = themeMode;
}
