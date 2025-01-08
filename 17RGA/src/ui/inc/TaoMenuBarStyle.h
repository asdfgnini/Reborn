#ifndef TAOMENUBARSTYLE_H
#define TAOMENUBARSTYLE_H

#include <QProxyStyle>


#include "../../include/Def.h"


class TaoMenuBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TaoMenuBarStyle(QStyle* style = nullptr);
    ~TaoMenuBarStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
    int _menuBarItemMargin{0};
};

#endif // TAOMENUBARSTYLE_H
