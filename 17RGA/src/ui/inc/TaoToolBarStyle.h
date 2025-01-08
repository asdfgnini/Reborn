#ifndef TAOTOOLBARSTYLE_H
#define TAOTOOLBARSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"

class QStyleOptionToolButton;
class TaoToolBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TaoToolBarStyle(QStyle* style = nullptr);
    ~TaoToolBarStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
    void _drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void _drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void _drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const;
};

#endif // TAOTOOLBARSTYLE_H
