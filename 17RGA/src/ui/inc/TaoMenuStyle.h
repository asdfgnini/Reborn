#ifndef TAOMENUSTYLE_H
#define TAOMENUSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"

class TaoMenuStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, MenuItemHeight);

public:
    explicit TaoMenuStyle(QStyle* style = nullptr);
    ~TaoMenuStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

private:
    mutable bool _isAnyoneItemHasIcon{false};
    int _shadowBorderWidth{6};
    TaoThemeType::ThemeMode _themeMode;
    int _iconWidth{22};
};

#endif // TAOMENUSTYLE_H
