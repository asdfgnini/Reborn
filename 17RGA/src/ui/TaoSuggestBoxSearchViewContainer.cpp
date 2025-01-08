#include "./inc/TaoSuggestBoxSearchViewContainer.h"

#include <QPainter>

#include "./inc/TaoTheme.h"

TaoSuggestBoxSearchViewContainer::TaoSuggestBoxSearchViewContainer(QWidget* parent)
    : QWidget{parent}
{
    setContentsMargins(8, 8, 8, 8);
    setObjectName("TaoSuggestBoxSearchViewBaseWidget");
    setStyleSheet("#TaoSuggestBoxSearchViewBaseWidget{background-color:transparent}");
    _themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

TaoSuggestBoxSearchViewContainer::~TaoSuggestBoxSearchViewContainer()
{
}

void TaoSuggestBoxSearchViewContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    tTheme->drawEffectShadow(&painter, rect(), 6, 8);
    painter.setPen(TaoThemeColor(_themeMode, PopupBorder));
    painter.setBrush(TaoThemeColor(_themeMode, PopupBase));
    QRect foregroundRect(6, 0, rect().width() - 2 * 6, rect().height() - 6);
    painter.drawRoundedRect(foregroundRect, 8, 8);
    painter.restore();
}
