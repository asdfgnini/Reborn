#ifndef TAOTOGGLEBUTTONPRIVATE_H
#define TAOTOGGLEBUTTONPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoToggleButton;
class TaoToggleButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoToggleButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Text)
    Q_PROPERTY_CREATE(int, ToggleAlpha)
public:
    explicit TaoToggleButtonPrivate(QObject* parent = nullptr);
    ~TaoToggleButtonPrivate();

private:
    bool _isAlphaAnimationFinished{true};
    bool _isToggled{false};
    bool _isPressed{false};
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOTOGGLEBUTTONPRIVATE_H
