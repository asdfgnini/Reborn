#include "./inc/TaoScrollPageArea.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "./inc/TaoTheme.h"
#include "private/TaoScrollPageAreaPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(TaoScrollPageArea, int, BorderRadius)
TaoScrollPageArea::TaoScrollPageArea(QWidget* parent)
    : QWidget{parent}, d_ptr(new TaoScrollPageAreaPrivate())
{
    Q_D(TaoScrollPageArea);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    setFixedHeight(75);
    d->_themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

TaoScrollPageArea::~TaoScrollPageArea()
{
}

void TaoScrollPageArea::paintEvent(QPaintEvent* event)
{
    Q_D(TaoScrollPageArea);
    QPainter painter(this);
    painter.save();
    painter.setPen(TaoThemeColor(d->_themeMode, BasicBorder));
    painter.setBrush(TaoThemeColor(d->_themeMode, BasicBaseAlpha));
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    painter.restore();
}
