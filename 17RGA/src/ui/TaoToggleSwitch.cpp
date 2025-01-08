#include "./inc/TaoToggleSwitch.h"

#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "./inc/TaoTheme.h"
#include "private/TaoToggleSwitchPrivate.h"


TaoToggleSwitch::TaoToggleSwitch(QWidget* parent)
    : QWidget{parent}, d_ptr(new TaoToggleSwitchPrivate())
{
    Q_D(TaoToggleSwitch);
    d->q_ptr = this;
    setMouseTracking(true);
    setFixedSize(44, 22);
    d->_circleCenterX = -1;
    d->_isToggled = false;
    d->_themeMode = tTheme->getThemeMode();
    setProperty("circleCenterX", 0.01);
    setProperty("circleRadius", 0.01);
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

TaoToggleSwitch::~TaoToggleSwitch()
{
}

void TaoToggleSwitch::setIsToggled(bool isToggled)
{
    Q_D(TaoToggleSwitch);
    if (d->_isToggled == isToggled)
    {
        return;
    }
    if (d->_isToggled)
    {
        d->_startPosAnimation(width() - height() / 2 - d->_margin * 2, height() / 2, isToggled);
    }
    else
    {
        d->_startPosAnimation(height() / 2, width() - height() / 2 - d->_margin * 2, isToggled);
    }
}

bool TaoToggleSwitch::getIsToggled() const
{
    return d_ptr->_isToggled;
}

bool TaoToggleSwitch::event(QEvent* event)
{
    Q_D(TaoToggleSwitch);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        if (isEnabled())
        {
            d->_startRadiusAnimation(height() * 0.3, height() * 0.35);
        }
        break;
    }
    case QEvent::Leave:
    {
        if (isEnabled())
        {
            d->_startRadiusAnimation(height() * 0.35, height() * 0.3);
        }
        break;
    }
    case QEvent::MouseMove:
    {
        update();
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void TaoToggleSwitch::mousePressEvent(QMouseEvent* event)
{
    Q_D(TaoToggleSwitch);
    d->_adjustCircleCenterX();
    d->_isLeftButtonPress = true;
    d->_lastMouseX = event->pos().x();
    d->_startRadiusAnimation(d->_circleRadius, height() * 0.25);
    QWidget::mousePressEvent(event);
}

void TaoToggleSwitch::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(TaoToggleSwitch);
    d->_isLeftButtonPress = false;
    QWidget::mouseReleaseEvent(event);
    if (d->_isMousePressMove)
    {
        d->_isMousePressMove = false;
        if (d->_circleCenterX > width() / 2)
        {
            d->_startPosAnimation(d->_circleCenterX, width() - height() / 2 - d->_margin * 2, true);
        }
        else
        {
            d->_startPosAnimation(d->_circleCenterX, height() / 2, false);
        }
    }
    else
    {
        if (d->_isToggled)
        {
            d->_startPosAnimation(d->_circleCenterX, height() / 2, false);
        }
        else
        {
            d->_startPosAnimation(d->_circleCenterX, width() - height() / 2 - d->_margin * 2, true);
        }
    }
    d->_startRadiusAnimation(height() * 0.25, height() * 0.35);
}

void TaoToggleSwitch::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(TaoToggleSwitch);
    if (d->_isLeftButtonPress)
    {
        d->_isMousePressMove = true;
        int moveX = event->pos().x() - d->_lastMouseX;
        d->_lastMouseX = event->pos().x();
        d->_circleCenterX += moveX;
        d->_adjustCircleCenterX();
    }
    QWidget::mouseMoveEvent(event);
}

void TaoToggleSwitch::paintEvent(QPaintEvent* event)
{
    Q_D(TaoToggleSwitch);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 背景绘制
    painter.setPen(d->_isToggled ? QPen(TaoThemeColor(d->_themeMode, BasicBorder), 1.5) : QPen(TaoThemeColor(d->_themeMode, BasicBorderDeep), 1.5));
    painter.setBrush(isEnabled() ? d->_isToggled ? TaoThemeColor(d->_themeMode, PrimaryNormal) : (underMouse() ? TaoThemeColor(d->_themeMode, BasicHover) : TaoThemeColor(d->_themeMode, BasicBase)) : TaoThemeColor(d->_themeMode, BasicDisable));
    QPainterPath path;
    path.moveTo(width() - height() - d->_margin, height() - d->_margin);
    path.arcTo(QRectF(QPointF(width() - height() - d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), -90, 180);
    path.lineTo(height() / 2 + d->_margin, d->_margin);
    path.arcTo(QRectF(QPointF(d->_margin, d->_margin), QSize(height() - d->_margin * 2, height() - d->_margin * 2)), 90, 180);
    path.lineTo(width() - height() - d->_margin, height() - d->_margin);
    path.closeSubpath();
    painter.drawPath(path);

    // 圆心绘制
    painter.setPen(Qt::NoPen);
    painter.setBrush(isEnabled() ? d->_isToggled ? TaoThemeColor(d->_themeMode, BasicTextInvert) : TaoThemeColor(d->_themeMode, ToggleSwitchNoToggledCenter) : TaoThemeColor(d->_themeMode, BasicTextDisable));
    if (d->_circleRadius == 0)
    {
        d->_circleRadius = this->isEnabled() ? (underMouse() ? height() * 0.35 : height() * 0.3) : height() * 0.3;
    }
    if (d->_isLeftButtonPress)
    {
        painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius);
    }
    else
    {
        if (d->_circleCenterX == -1)
        {
            d->_circleCenterX = d->_isToggled ? width() - height() / 2 - d->_margin * 2 : height() / 2;
        }
        painter.drawEllipse(QPointF(d->_circleCenterX, height() / 2), d->_circleRadius, d->_circleRadius);
    }
    painter.restore();
}
