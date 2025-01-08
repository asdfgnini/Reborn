#include "./inc/TaoTheme.h"

#include <QPainter>
#include <QPainterPath>

#include "./private/TaoThemePrivate.h"
Q_SINGLETON_CREATE_CPP(TaoTheme)
TaoTheme::TaoTheme(QObject* parent)
    : QObject{parent}, d_ptr(new TaoThemePrivate())
{
    Q_D(TaoTheme);
    d->q_ptr = this;
    d->_initThemeColor();
}

TaoTheme::~TaoTheme()
{
}

void TaoTheme::setThemeMode(TaoThemeType::ThemeMode themeMode)
{
    Q_D(TaoTheme);
    d->_themeMode = themeMode;
    Q_EMIT themeModeChanged(d->_themeMode);
}

TaoThemeType::ThemeMode TaoTheme::getThemeMode() const
{
    Q_D(const TaoTheme);
    return d->_themeMode;
}

void TaoTheme::drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius)
{
    Q_D(TaoTheme);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QColor color = d->_themeMode == TaoThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
    for (int i = 0; i < shadowBorderWidth; i++)
    {
        path.addRoundedRect(shadowBorderWidth - i, shadowBorderWidth - i, widgetRect.width() - (shadowBorderWidth - i) * 2, widgetRect.height() - (shadowBorderWidth - i) * 2, borderRadius + i, borderRadius + i);
        int alpha = 1 * (shadowBorderWidth - i + 1);
        color.setAlpha(alpha > 255 ? 255 : alpha);
        painter->setPen(color);
        painter->drawPath(path);
    }
    painter->restore();
}

void TaoTheme::setThemeColor(TaoThemeType::ThemeMode themeMode, TaoThemeType::ThemeColor themeColor, QColor newColor)
{
    Q_D(TaoTheme);
    if (themeMode == TaoThemeType::Light)
    {
        d->_lightThemeColorList[themeColor] = newColor;
    }
    else
    {
        d->_darkThemeColorList[themeColor] = newColor;
    }
}

const QColor& TaoTheme::getThemeColor(TaoThemeType::ThemeMode themeMode, TaoThemeType::ThemeColor themeColor)
{
    Q_D(TaoTheme);
    if (themeMode == TaoThemeType::Light)
    {
        return d->_lightThemeColorList[themeColor];
    }
    else
    {
        return d->_darkThemeColorList[themeColor];
    }
}
