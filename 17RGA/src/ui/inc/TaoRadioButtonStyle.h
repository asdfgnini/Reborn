#ifndef TAORADIOBUTTONSTYLE_H
#define TAORADIOBUTTONSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"
class TaoRadioButtonStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TaoRadioButtonStyle(QStyle* style = nullptr);
    ~TaoRadioButtonStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAORADIOBUTTONSTYLE_H
