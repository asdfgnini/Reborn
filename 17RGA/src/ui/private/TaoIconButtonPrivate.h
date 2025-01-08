#ifndef TAOICONBUTTONPRIVATE_H
#define TAOICONBUTTONPRIVATE_H

#include <QColor>
#include <QObject>
#include <QPixmap>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoIconButton;
class TaoIconButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoIconButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(qreal, Opacity);
    Q_PROPERTY_CREATE_D(TaoIconType::IconName, Awesome)
    Q_PROPERTY_CREATE_D(QColor, LightHoverColor);
    Q_PROPERTY_CREATE_D(QColor, DarkHoverColor);
    Q_PROPERTY_CREATE_D(QColor, LightIconColor);
    Q_PROPERTY_CREATE_D(QColor, DarkIconColor);
    Q_PROPERTY_CREATE_D(QColor, LightHoverIconColor);
    Q_PROPERTY_CREATE_D(QColor, DarkHoverIconColor);
    Q_PROPERTY_CREATE_D(bool, IsSelected);
    Q_PROPERTY_CREATE(int, HoverAlpha)
public:
    explicit TaoIconButtonPrivate(QObject* parent = nullptr);
    ~TaoIconButtonPrivate();

private:
    QPixmap _iconPix;
    bool _isAlphaAnimationFinished{true};
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOICONBUTTONPRIVATE_H
