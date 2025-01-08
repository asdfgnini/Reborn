#include "TaoToggleSwitchPrivate.h"

#include <QPropertyAnimation>

#include "../inc/TaoToggleSwitch.h"

TaoToggleSwitchPrivate::TaoToggleSwitchPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoToggleSwitchPrivate::~TaoToggleSwitchPrivate()
{
}

void TaoToggleSwitchPrivate::_startPosAnimation(qreal startX, qreal endX, bool isToggle)
{
    Q_Q(TaoToggleSwitch);
    QPropertyAnimation* circleAnimation = new QPropertyAnimation(q, "circleCenterX");
    connect(circleAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
                this->_circleCenterX = value.toReal();
                q->update(); });
    circleAnimation->setStartValue(startX);
    circleAnimation->setEndValue(endX);
    circleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    _isToggled = isToggle;
    Q_EMIT q->toggled(isToggle);
}

void TaoToggleSwitchPrivate::_startRadiusAnimation(qreal startRadius, qreal endRadius)
{
    Q_Q(TaoToggleSwitch);
    QPropertyAnimation* circleRadiusAnimation = new QPropertyAnimation(q, "circleRadius");
    connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, q, [=](const QVariant& value) {
        this->_circleRadius = value.toReal();
        q->update(); });
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleRadiusAnimation->setStartValue(startRadius);
    circleRadiusAnimation->setEndValue(endRadius);
    circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void TaoToggleSwitchPrivate::_adjustCircleCenterX()
{
    Q_Q(TaoToggleSwitch);
    if (_circleCenterX > q->width() - q->height() / 2 - _margin * 2)
    {
        _circleCenterX = q->width() - q->height() / 2 - _margin * 2;
    }
    if (_circleCenterX < q->height() / 2)
    {
        _circleCenterX = q->height() / 2;
    }
}
