#include "./inc/TaoScrollArea.h"

#include <QScrollBar>
#include <QScroller>

#include "private/TaoScrollAreaPrivate.h"
#include "./inc/TaoScrollBar.h"

TaoScrollArea::TaoScrollArea(QWidget* parent)
    : QScrollArea(parent), d_ptr(new TaoScrollAreaPrivate())
{
    Q_D(TaoScrollArea);
    d->q_ptr = this;
    setObjectName("TaoScrollArea");
    setStyleSheet("#TaoScrollArea{background-color:transparent;border:0px;}");
    setHorizontalScrollBar(new TaoScrollBar(this));
    setVerticalScrollBar(new TaoScrollBar(this));
    QScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

TaoScrollArea::~TaoScrollArea()
{
}

void TaoScrollArea::setIsGrabGesture(bool isEnable, qreal mousePressEventDelay)
{
    if (isEnable)
    {
        QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
        QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
        properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, mousePressEventDelay);
        properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
        properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
        properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
        QScroller::scroller(this->viewport())->setScrollerProperties(properties);
    }
    else
    {
        QScroller::ungrabGesture(this->viewport());
    }
}

void TaoScrollArea::setIsOverShoot(Qt::Orientation orientation, bool isEnable)
{
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
    properties.setScrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy, isEnable ? QScrollerProperties::OvershootAlwaysOn : QScrollerProperties::OvershootAlwaysOff);
    QScroller::scroller(this->viewport())->setScrollerProperties(properties);
}

bool TaoScrollArea::getIsOverShoot(Qt::Orientation orientation) const
{
    QScrollerProperties properties = QScroller::scroller(this->viewport())->scrollerProperties();
    return properties.scrollMetric(orientation == Qt::Horizontal ? QScrollerProperties::HorizontalOvershootPolicy : QScrollerProperties::VerticalOvershootPolicy).toBool();
}

void TaoScrollArea::setIsAnimation(Qt::Orientation orientation, bool isAnimation)
{
    if (orientation == Qt::Horizontal)
    {
        dynamic_cast<TaoScrollBar*>(this->horizontalScrollBar())->setIsAnimation(isAnimation);
    }
    else
    {
        dynamic_cast<TaoScrollBar*>(this->verticalScrollBar())->setIsAnimation(isAnimation);
    }
}

bool TaoScrollArea::getIsAnimation(Qt::Orientation orientation) const
{
    if (orientation == Qt::Horizontal)
    {
        return dynamic_cast<TaoScrollBar*>(this->horizontalScrollBar())->getIsAnimation();
    }
    else
    {
        return dynamic_cast<TaoScrollBar*>(this->verticalScrollBar())->getIsAnimation();
    }
}
