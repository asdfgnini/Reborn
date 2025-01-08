#ifndef TAOWINDOWSTYLE_H
#define TAOWINDOWSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"

class TaoWindowStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TaoWindowStyle(QStyle* style = nullptr);
    ~TaoWindowStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOWINDOWSTYLE_H
