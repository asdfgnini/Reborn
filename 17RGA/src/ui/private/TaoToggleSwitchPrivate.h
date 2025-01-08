#ifndef TAOTOGGLESWITCHPRIVATE_H
#define TAOTOGGLESWITCHPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoToggleSwitch;
class TaoToggleSwitchPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoToggleSwitch);

public:
    explicit TaoToggleSwitchPrivate(QObject* parent = nullptr);
    ~TaoToggleSwitchPrivate();

private:
    bool _isToggled{false};
    int _margin{1};
    qreal _circleCenterX{0};
    qreal _circleRadius{0};
    bool _isLeftButtonPress{false};
    bool _isMousePressMove{false};
    int _lastMouseX{0};
    TaoThemeType::ThemeMode _themeMode;
    void _startPosAnimation(qreal startX, qreal endX, bool isToggle);
    void _startRadiusAnimation(qreal startRadius, qreal endRadius);
    void _adjustCircleCenterX();
};

#endif // TAOTOGGLESWITCHPRIVATE_H
