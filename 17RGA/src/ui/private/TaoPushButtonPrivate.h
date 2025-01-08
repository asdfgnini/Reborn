#ifndef ELAPUSHBUTTONPRIVATE_H
#define ELAPUSHBUTTONPRIVATE_H

#include <QColor>
#include <QObject>


#include "../../include/Def.h"
#include "../../include/stdafx.h"
class TaoPushButton;
class TaoPushButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoPushButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QColor, LightDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, DarkDefaultColor)
    Q_PROPERTY_CREATE_D(QColor, LightHoverColor)
    Q_PROPERTY_CREATE_D(QColor, DarkHoverColor)
    Q_PROPERTY_CREATE_D(QColor, LightPressColor)
    Q_PROPERTY_CREATE_D(QColor, DarkPressColor)
public:
    explicit TaoPushButtonPrivate(QObject* parent = nullptr);
    ~TaoPushButtonPrivate();

private:
    QColor _lightTextColor;
    QColor _darkTextColor;
    bool _isPressed{false};
    int _shadowBorderWidth{3};
    TaoThemeType::ThemeMode _themeMode;
};

#endif // ELAPUSHBUTTONPRIVATE_H
