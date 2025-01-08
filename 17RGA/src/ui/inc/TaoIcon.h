#ifndef TAOICON_H
#define TAOICON_H
#include <QIcon>

#include "../../include/Def.h"
#include "../../include/stdafx.h"
#include "../../include/singleton.h"
class TAO_EXPORT TaoIcon
{
    Q_SINGLETON_CREATE_H(TaoIcon)
private:
    explicit TaoIcon();
    ~TaoIcon();

public:
    QIcon getTaoIcon(TaoIconType::IconName awesome);
    QIcon getTaoIcon(TaoIconType::IconName awesome, QColor iconColor);
    QIcon getTaoIcon(TaoIconType::IconName awesome, int pixelSize);
    QIcon getTaoIcon(TaoIconType::IconName awesome, int pixelSize, QColor iconColor);
    QIcon getTaoIcon(TaoIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight);
    QIcon getTaoIcon(TaoIconType::IconName awesome, int pixelSize, int fixedWidth, int fixedHeight, QColor iconColor);
};

#endif // TAOICON_H
