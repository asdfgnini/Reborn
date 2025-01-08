#include "./inc/TaoIconButton.h"

#include <QEvent>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "./inc/TaoTheme.h"
#include "private/TaoIconButtonPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, qreal, Opacity);
Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, QColor, LightHoverColor);
Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, QColor, DarkHoverColor);
Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, QColor, LightIconColor);
Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, QColor, DarkIconColor);
Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, QColor, LightHoverIconColor);
Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, QColor, DarkHoverIconColor);
Q_PROPERTY_CREATE_Q_CPP(TaoIconButton, bool, IsSelected);
TaoIconButton::TaoIconButton(QPixmap pix, QWidget* parent)
    : QPushButton(parent), d_ptr(new TaoIconButtonPrivate())
{
    Q_D(TaoIconButton);
    d->q_ptr = this;
    d->_iconPix = pix.copy();
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = TaoThemeColor(TaoThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = TaoThemeColor(TaoThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_pDarkIconColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = tTheme->getThemeMode();
    connect(this, &TaoIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

TaoIconButton::TaoIconButton(TaoIconType::IconName awesome, QWidget* parent)
    : QPushButton(parent), d_ptr(new TaoIconButtonPrivate())
{
    Q_D(TaoIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = TaoThemeColor(TaoThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = TaoThemeColor(TaoThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_pDarkIconColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = tTheme->getThemeMode();
    QFont iconFont = QFont("TaoAwesome");
    iconFont.setPixelSize(15);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    connect(this, &TaoIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

TaoIconButton::TaoIconButton(TaoIconType::IconName awesome, int pixelSize, QWidget* parent)
    : QPushButton(parent), d_ptr(new TaoIconButtonPrivate())
{
    Q_D(TaoIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = TaoThemeColor(TaoThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = TaoThemeColor(TaoThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_pDarkIconColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = tTheme->getThemeMode();
    QFont iconFont = QFont("TaoAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    connect(this, &TaoIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

TaoIconButton::TaoIconButton(TaoIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QWidget* parent)
    : QPushButton(parent), d_ptr(new TaoIconButtonPrivate())
{
    Q_D(TaoIconButton);
    d->q_ptr = this;
    d->_pHoverAlpha = 0;
    d->_pOpacity = 1;
    d->_pLightHoverColor = TaoThemeColor(TaoThemeType::Light, BasicHoverAlpha);
    d->_pDarkHoverColor = TaoThemeColor(TaoThemeType::Dark, BasicHoverAlpha);
    d->_pLightIconColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_pDarkIconColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    d->_pLightHoverIconColor = TaoThemeColor(TaoThemeType::Light, BasicText);
    d->_pDarkHoverIconColor = TaoThemeColor(TaoThemeType::Dark, BasicText);
    d->_pIsSelected = false;
    d->_pBorderRadius = 0;
    d->_themeMode = tTheme->getThemeMode();
    QFont iconFont = QFont("TaoAwesome");
    iconFont.setPixelSize(pixelSize);
    this->setFont(iconFont);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
    this->setFixedSize(fixedWidth, fixedHeight);
    connect(this, &TaoIconButton::pIsSelectedChanged, this, [=]() { update(); });
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

TaoIconButton::~TaoIconButton()
{
}

void TaoIconButton::setAwesome(TaoIconType::IconName awesome)
{
    Q_D(TaoIconButton);
    d->_pAwesome = awesome;
    this->setText(QChar((unsigned short)awesome));
}

TaoIconType::IconName TaoIconButton::getAwesome() const
{
    return this->d_ptr->_pAwesome;
}

void TaoIconButton::setPixmap(QPixmap pix)
{
    Q_D(TaoIconButton);
    d->_iconPix = pix.copy();
}

bool TaoIconButton::event(QEvent* event)
{
    Q_D(TaoIconButton);
    switch (event->type())
    {
    case QEvent::Enter:
    {
        if (isEnabled() && !d->_pIsSelected)
        {
            d->_isAlphaAnimationFinished = false;
            QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pHoverAlpha");
            connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_isAlphaAnimationFinished = true;
            });
            alphaAnimation->setDuration(175);
            alphaAnimation->setStartValue(d->_pHoverAlpha);
            alphaAnimation->setEndValue(d->_themeMode == TaoThemeType::Light ? d->_pLightHoverColor.alpha() : d->_pDarkHoverColor.alpha());
            alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
    }
    case QEvent::Leave:
    {
        if (isEnabled() && !d->_pIsSelected)
        {
            d->_isAlphaAnimationFinished = false;
            QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pHoverAlpha");
            connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_isAlphaAnimationFinished = true;
            });
            alphaAnimation->setDuration(175);
            alphaAnimation->setStartValue(d->_pHoverAlpha);
            alphaAnimation->setEndValue(0);
            alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QPushButton::event(event);
}

void TaoIconButton::paintEvent(QPaintEvent* event)
{
    Q_D(TaoIconButton);
    QPainter painter(this);
    painter.save();
    painter.setOpacity(d->_pOpacity);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    if (d->_isAlphaAnimationFinished || d->_pIsSelected)
    {
        painter.setBrush(d->_pIsSelected ? d->_themeMode == TaoThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor
                         : isEnabled()   ? underMouse() ? d->_themeMode == TaoThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor : Qt::transparent
                                         : Qt::transparent);
    }
    else
    {
        QColor hoverColor = d->_themeMode == TaoThemeType::Light ? d->_pLightHoverColor : d->_pDarkHoverColor;
        hoverColor.setAlpha(d->_pHoverAlpha);
        painter.setBrush(hoverColor);
    }
    painter.drawRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
    // 图标绘制
    if (!d->_iconPix.isNull())
    {
        QPainterPath path;
        path.addEllipse(rect());
        painter.setClipPath(path);
        painter.drawPixmap(rect(), d->_iconPix);
    }
    else
    {
        painter.setPen(isEnabled() ? d->_themeMode == TaoThemeType::Light ? underMouse() ? d->_pLightHoverIconColor : d->_pLightIconColor : underMouse() ? d->_pDarkHoverIconColor
                                                                                                                                                         : d->_pDarkIconColor
                                   : TaoThemeColor(d->_themeMode, BasicTextDisable));
        painter.drawText(rect(), Qt::AlignCenter, QChar((unsigned short)d->_pAwesome));
    }
    painter.restore();
}
