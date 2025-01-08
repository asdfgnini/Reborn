#ifndef TAOINTERACTIVECARDPRIVATE_H
#define TAOINTERACTIVECARDPRIVATE_H

#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QSize>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoInteractiveCard;
class TaoInteractiveCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoInteractiveCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(QString, Title);
    Q_PROPERTY_CREATE_D(QString, SubTitle);
    Q_PROPERTY_CREATE_D(int, TitlePixelSize);
    Q_PROPERTY_CREATE_D(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_D(int, TitleSpacing);
    Q_PROPERTY_CREATE_D(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_D(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_D(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_D(TaoCardPixType::PixMode, CardPixMode);

public:
    explicit TaoInteractiveCardPrivate(QObject* parent = nullptr);
    ~TaoInteractiveCardPrivate();

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOINTERACTIVECARDPRIVATE_H
