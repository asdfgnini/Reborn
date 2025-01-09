#include "./inc/TaoMessageButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "./inc/TaoMessageBar.h"
#include "./inc/TaoTheme.h"
#include "private/TaoMessageButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(TaoMessageButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(TaoMessageButton, QString, BarTitle);
Q_PROPERTY_CREATE_Q_CPP(TaoMessageButton, QString, BarText);
Q_PROPERTY_CREATE_Q_CPP(TaoMessageButton, int, DisplayMsec);
Q_PROPERTY_CREATE_Q_CPP(TaoMessageButton, QWidget*, MessageTargetWidget)
Q_PROPERTY_CREATE_Q_CPP(TaoMessageButton, TaoMessageBarType::MessageMode, MessageMode);
Q_PROPERTY_CREATE_Q_CPP(TaoMessageButton, TaoMessageBarType::PositionPolicy, PositionPolicy);

TaoMessageButton::TaoMessageButton(QWidget* parent)
    : QPushButton(parent), d_ptr(new TaoMessageButtonPrivate())
{
    Q_D(TaoMessageButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    setMouseTracking(true);
    setFixedSize(80, 38);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setText("Message");
    setObjectName("TaoMessageButton");
    setStyleSheet("#TaoMessageButton{background-color:transparent;}");
    d->_pDisplayMsec = 2000;
    d->_pMessageMode = TaoMessageBarType::Success;
    d->_pPositionPolicy = TaoMessageBarType::TopRight;
    d->_themeMode = tTheme->getThemeMode();
    d->_pMessageTargetWidget = nullptr;
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
    connect(this, &TaoMessageButton::clicked, this, [=]() {
        switch(d->_pMessageMode)
        {
        case TaoMessageBarType::Success:
        {
            TaoMessageBar::success(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,d->_pMessageTargetWidget);
            break;
        }
        case TaoMessageBarType::Warning:
        {
            TaoMessageBar::warning(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,d->_pMessageTargetWidget);
            break;
        }
        case TaoMessageBarType::Information:
        {
            TaoMessageBar::information(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,d->_pMessageTargetWidget);
            break;
        }
        case TaoMessageBarType::Error:
        {
            TaoMessageBar::error(d->_pPositionPolicy,d->_pBarTitle,d->_pBarText,d->_pDisplayMsec,d->_pMessageTargetWidget);
            break;
        }
        } });
}

TaoMessageButton::TaoMessageButton(QString text, QWidget* parent)
    : TaoMessageButton(parent)
{
    setText(text);
}

TaoMessageButton::~TaoMessageButton()
{
}

void TaoMessageButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(TaoMessageButton);
    if (event->button() == Qt::LeftButton)
    {
        d->_isLeftButtonPress = true;
    }
    QPushButton::mousePressEvent(event);
}

void TaoMessageButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(TaoMessageButton);
    d->_isLeftButtonPress = false;
    QPushButton::mouseReleaseEvent(event);
}

void TaoMessageButton::paintEvent(QPaintEvent* event)
{
    Q_D(TaoMessageButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 高性能阴影
    tTheme->drawEffectShadow(&painter, rect(), d->_shadowBorderWidth, d->_pBorderRadius);

    // 背景绘制
    painter.save();
    QRect foregroundRect(d->_penBorderWidth + d->_shadowBorderWidth, d->_penBorderWidth + d->_shadowBorderWidth, width() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth), height() - 2 * (d->_penBorderWidth + d->_shadowBorderWidth));
    painter.setPen(QPen(TaoThemeColor(d->_themeMode, BasicBorder), d->_penBorderWidth));
    painter.setBrush(isEnabled() ? underMouse() ? TaoThemeColor(d->_themeMode, BasicHover) : TaoThemeColor(d->_themeMode, BasicBase) : TaoThemeColor(d->_themeMode, BasicDisable));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    //文字绘制
    painter.setPen(isEnabled() ? d->_isLeftButtonPress ? TaoThemeColor(d->_themeMode, BasicTextPress) : TaoThemeColor(d->_themeMode, BasicText) : TaoThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(rect(), Qt::AlignCenter, text());
    painter.restore();
}
