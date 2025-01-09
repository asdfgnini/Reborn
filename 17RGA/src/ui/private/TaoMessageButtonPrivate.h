#ifndef TAOMESSAGEBUTTONPRIVATE_H
#define TAOMESSAGEBUTTONPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoMessageButton;
class TaoMessageButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoMessageButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, BarTitle);
    Q_PROPERTY_CREATE_D(QString, BarText);
    Q_PROPERTY_CREATE_D(int, DisplayMsec);
    Q_PROPERTY_CREATE_D(QWidget*, MessageTargetWidget)
    Q_PROPERTY_CREATE_D(TaoMessageBarType::MessageMode, MessageMode);
    Q_PROPERTY_CREATE_D(TaoMessageBarType::PositionPolicy, PositionPolicy);

public:
    explicit TaoMessageButtonPrivate(QObject* parent = nullptr);
    ~TaoMessageButtonPrivate();

private:
    bool _isLeftButtonPress{false};
    int _shadowBorderWidth{3};
    int _penBorderWidth{1};
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOMESSAGEBUTTONPRIVATE_H
