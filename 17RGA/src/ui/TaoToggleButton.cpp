#include "./inc/TaoToggleButton.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "./inc/TaoTheme.h"
#include "private/TaoToggleButtonPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(TaoToggleButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(TaoToggleButton, QString, Text)
TaoToggleButton::TaoToggleButton(QWidget* parent)
    : QWidget(parent), d_ptr(new TaoToggleButtonPrivate())
{
    Q_D(TaoToggleButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = tTheme->getThemeMode();
    d->_pToggleAlpha = 0;
    setMouseTracking(true);
    setFixedSize(80, 32);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("TaoToggleButton");
    setStyleSheet("#TaoToggleButton{background-color:transparent;}");
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

TaoToggleButton::TaoToggleButton(QString text, QWidget* parent)
    : TaoToggleButton(parent)
{
    Q_D(TaoToggleButton);
    d->_pText = text;
}

TaoToggleButton::~TaoToggleButton()
{
}

void TaoToggleButton::setIsToggled(bool isToggled)
{
    Q_D(TaoToggleButton);
    d->_isToggled = isToggled;
    d->_pToggleAlpha = isToggled ? 255 : 0;
    update();
    Q_EMIT toggled(isToggled);
}

bool TaoToggleButton::getIsToggled() const
{
    Q_D(const TaoToggleButton);
    return d->_isToggled;
}

bool TaoToggleButton::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Enter:
    case QEvent::Leave:
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

void TaoToggleButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(TaoToggleButton);
    d->_isPressed = true;
    update();
    QWidget::mouseReleaseEvent(event);
}

void TaoToggleButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(TaoToggleButton);
    d->_isPressed = false;
    d->_isAlphaAnimationFinished = false;
    d->_isToggled = !d->_isToggled;
    QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pToggleAlpha");
    connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->_isAlphaAnimationFinished = true;
    });
    alphaAnimation->setDuration(250);
    alphaAnimation->setStartValue(d->_pToggleAlpha);
    if (d->_isToggled)
    {
        alphaAnimation->setEndValue(255);
    }
    else
    {
        alphaAnimation->setEndValue(0);
    }
    alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    Q_EMIT this->toggled(d->_isToggled);
    QWidget::mouseReleaseEvent(event);
}

void TaoToggleButton::paintEvent(QPaintEvent* event)
{
    Q_D(TaoToggleButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.save();
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    if (d->_isAlphaAnimationFinished)
    {
        if (d->_isToggled)
        {
            painter.setPen(TaoThemeColor(d->_themeMode, BasicBorder));
            painter.setBrush(isEnabled() ? d->_isPressed ? TaoThemeColor(d->_themeMode, PrimaryPress) : (underMouse() ? TaoThemeColor(d->_themeMode, PrimaryHover) : TaoThemeColor(d->_themeMode, PrimaryNormal)) : TaoThemeColor(d->_themeMode, BasicDisable));
        }
        else
        {
            painter.setPen(TaoThemeColor(d->_themeMode, BasicBorder));
            painter.setBrush(isEnabled() ? d->_isPressed ? TaoThemeColor(d->_themeMode, BasicPress) : (underMouse() ? TaoThemeColor(d->_themeMode, BasicHover) : TaoThemeColor(d->_themeMode, BasicBase)) : TaoThemeColor(d->_themeMode, BasicDisable));
        }
    }
    else
    {
        painter.setPen(Qt::NoPen);
        QColor toggleColor = TaoThemeColor(d->_themeMode, PrimaryNormal);
        toggleColor.setAlpha(d->_pToggleAlpha);
        painter.setBrush(toggleColor);
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed && !d->_isToggled)
    {
        painter.setPen(TaoThemeColor(d->_themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - 1, foregroundRect.x() + foregroundRect.width() - d->_pBorderRadius, height() - 1);
    }

    //文字绘制
    painter.setPen(isEnabled() ? d->_isToggled ? TaoThemeColor(d->_themeMode, BasicTextInvert) : TaoThemeColor(d->_themeMode, BasicText) : TaoThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, d->_pText);
    painter.restore();
}
