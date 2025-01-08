#include "./inc/TaoPushButton.h"

#include <QPainter>
#include <QPainterPath>

#include "./inc/TaoTheme.h"
#include "./private/TaoPushButtonPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(TaoPushButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(TaoPushButton, QColor, LightDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(TaoPushButton, QColor, DarkDefaultColor)
Q_PROPERTY_CREATE_Q_CPP(TaoPushButton, QColor, LightHoverColor)
Q_PROPERTY_CREATE_Q_CPP(TaoPushButton, QColor, DarkHoverColor)
Q_PROPERTY_CREATE_Q_CPP(TaoPushButton, QColor, LightPressColor)
Q_PROPERTY_CREATE_Q_CPP(TaoPushButton, QColor, DarkPressColor)
TaoPushButton::TaoPushButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new TaoPushButtonPrivate())
{
    Q_D(TaoPushButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = tTheme->getThemeMode();
    d->_pLightDefaultColor = TaoThemeColor(TaoThemeType::Light, BasicBase);
    d->_pDarkDefaultColor = TaoThemeColor(TaoThemeType::Dark, BasicBase);
    d->_pLightHoverColor = TaoThemeColor(TaoThemeType::Light, BasicHover);
    d->_pDarkHoverColor = TaoThemeColor(TaoThemeType::Dark, BasicHover);
    d->_pLightPressColor = TaoThemeColor(TaoThemeType::Light, BasicPress);
    d->_pDarkPressColor = TaoThemeColor(TaoThemeType::Dark, BasicPress);
    d->_lightTextColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_darkTextColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    setMouseTracking(true);
    setFixedHeight(38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("TaoPushButton");
    setStyleSheet("#TaoPushButton{background-color:transparent;}");
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

TaoPushButton::TaoPushButton(QString text, QWidget* parent)
    : TaoPushButton(parent)
{
    setText(text);
}

TaoPushButton::~TaoPushButton()
{
}

void TaoPushButton::setLightTextColor(QColor color)
{
    Q_D(TaoPushButton);
    d->_lightTextColor = color;
}

QColor TaoPushButton::getLightTextColor() const
{
    Q_D(const TaoPushButton);
    return d->_lightTextColor;
}

void TaoPushButton::setDarkTextColor(QColor color)
{
    Q_D(TaoPushButton);
    d->_darkTextColor = color;
}

QColor TaoPushButton::getDarkTextColor() const
{
    Q_D(const TaoPushButton);
    return d->_darkTextColor;
}

void TaoPushButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(TaoPushButton);
    d->_isPressed = true;
    QPushButton::mousePressEvent(event);
}

void TaoPushButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(TaoPushButton);
    d->_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

void TaoPushButton::paintEvent(QPaintEvent* event)
{
    Q_D(TaoPushButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    tTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_shadowBorderWidth, d->_shadowBorderWidth, width() - 2 * (d->_shadowBorderWidth), height() - 2 * d->_shadowBorderWidth);
    if (d->_themeMode == TaoThemeType::Light)
    {
        painter.setPen(TaoThemeColor(TaoThemeType::Light, BasicBorder));
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pLightPressColor : (underMouse() ? d->_pLightHoverColor : d->_pLightDefaultColor) : TaoThemeColor(d->_themeMode, BasicDisable));
    }
    else
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(isEnabled() ? d->_isPressed ? d->_pDarkPressColor : (underMouse() ? d->_pDarkHoverColor : d->_pDarkDefaultColor) : TaoThemeColor(d->_themeMode, BasicDisable));
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    if (!d->_isPressed)
    {
        painter.setPen(TaoThemeColor(d->_themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - d->_shadowBorderWidth, foregroundRect.width(), height() - d->_shadowBorderWidth);
    }
    //文字绘制
    painter.setPen(isEnabled() ? d->_themeMode == TaoThemeType::Light ? d->_lightTextColor : d->_darkTextColor : TaoThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, text());
    painter.restore();
}
