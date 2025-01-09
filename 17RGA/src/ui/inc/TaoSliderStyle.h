#ifndef TAOSLIDERSTYLE_H
#define TAOSLIDERSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"
class TaoSliderStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TaoSliderStyle(QStyle* style = nullptr);
    ~TaoSliderStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
    mutable QStyle::State _lastState{0};
    mutable qreal _circleRadius{0};
    void _startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const;
};

#endif // TAOSLIDERSTYLE_H
