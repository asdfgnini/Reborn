#include "./inc/TaoToolButton.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>

#include "./inc/TaoToolButtonStyle.h"
#include "./inc/TaoIcon.h"
#include "./inc/TaoMenu.h"
#include "./private/TaoToolButtonPrivate.h"
TaoToolButton::TaoToolButton(QWidget* parent)
    : QToolButton(parent), d_ptr(new TaoToolButtonPrivate())
{
    Q_D(TaoToolButton);
    d->q_ptr = this;
    setIconSize(QSize(22, 22));
    setPopupMode(QToolButton::InstantPopup);
    d->_toolButtonStyle = new TaoToolButtonStyle(style());
    setStyle(d->_toolButtonStyle);
}

TaoToolButton::~TaoToolButton()
{
}

void TaoToolButton::setBorderRadius(int borderRadius)
{
    Q_D(TaoToolButton);
    d->_toolButtonStyle->setBorderRadius(borderRadius);
    Q_EMIT pBorderRadiusChanged();
}

int TaoToolButton::getBorderRadius() const
{
    Q_D(const TaoToolButton);
    return d->_toolButtonStyle->getBorderRadius();
}

void TaoToolButton::setIsSelected(bool isSelected)
{
    Q_D(TaoToolButton);
    d->_toolButtonStyle->setIsSelected(isSelected);
    Q_EMIT pIsSelectedChanged();
}

bool TaoToolButton::getIsSelected() const
{
    Q_D(const TaoToolButton);
    return d->_toolButtonStyle->getIsSelected();
}

void TaoToolButton::setIsTransparent(bool isTransparent)
{
    Q_D(TaoToolButton);
    d->_toolButtonStyle->setIsTransparent(isTransparent);
    update();
}

bool TaoToolButton::getIsTransparent() const
{
    Q_D(const TaoToolButton);
    return d->_toolButtonStyle->getIsTransparent();
}

void TaoToolButton::setMenu(TaoMenu* menu)
{
    if (!menu || menu == this->menu())
    {
        return;
    }
    menu->setMenuItemHeight(27);
    QToolButton::setMenu(menu);
    menu->installEventFilter(this);
}

void TaoToolButton::setTaoIcon(TaoIconType::IconName icon)
{
    setProperty("TaoIconType", QChar((unsigned short)icon));
    setIcon(TaoIcon::getInstance()->getTaoIcon(TaoIconType::Broom, 1));
}

bool TaoToolButton::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(TaoToolButton);
    if (watched == menu())
    {
        switch (event->type())
        {
        case QEvent::Show:
        {
            //指示器动画
            QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d->_toolButtonStyle, "pExpandIconRotate");
            connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            rotateAnimation->setDuration(300);
            rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
            rotateAnimation->setStartValue(d->_toolButtonStyle->getExpandIconRotate());
            rotateAnimation->setEndValue(-180);
            rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        case QEvent::Hide:
        {
            //指示器动画
            QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d->_toolButtonStyle, "pExpandIconRotate");
            connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            rotateAnimation->setDuration(300);
            rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
            rotateAnimation->setStartValue(d->_toolButtonStyle->getExpandIconRotate());
            rotateAnimation->setEndValue(0);
            rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            QMouseEvent focusEvent(QEvent::MouseButtonPress, QPoint(-1, -1), QPoint(-1, -1), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
            QApplication::sendEvent(parentWidget(), &focusEvent);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return QObject::eventFilter(watched, event);
}
