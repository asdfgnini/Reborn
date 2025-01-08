#ifndef TAOSCROLLBARSTYLE_H
#define TAOSCROLLBARSTYLE_H
#include <QProxyStyle>

#include "../../include/Def.h"

class TaoScrollBar;
class TaoScrollBarStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool, IsExpand)
    Q_PROPERTY_CREATE(qreal, Opacity)
    Q_PROPERTY_CREATE(qreal, SliderExtent)
    Q_PRIVATE_CREATE(TaoScrollBar*, ScrollBar)
public:
    explicit TaoScrollBarStyle(QStyle* style = nullptr);
    ~TaoScrollBarStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;
    void startExpandAnimation(bool isExpand);

private:
    TaoThemeType::ThemeMode _themeMode;
    qreal _sliderMargin{2.5};
    int _scrollBarExtent{10};
};

#endif // TAOSCROLLBARSTYLE_H
