#ifndef TAOTOOLBUTTONSTYLE_H
#define TAOTOOLBUTTONSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"

class QStyleOptionToolButton;
class TaoToolButtonStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool, IsTransparent)
    Q_PRIVATE_CREATE(int, BorderRadius)
    Q_PROPERTY_CREATE(qreal, ExpandIconRotate)
    Q_PRIVATE_CREATE(bool, IsSelected);

public:
    explicit TaoToolButtonStyle(QStyle* style = nullptr);
    ~TaoToolButtonStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    void drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap);

private:
    TaoThemeType::ThemeMode _themeMode;
    int _contentMargin{6};
    int _hasMenuIndicatorWidth{30};
    void _drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void _drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const;
    void _drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const;
    qreal _calculateExpandIndicatorWidth(const QStyleOptionToolButton* bopt, QPainter* painter) const;
};

#endif // TAOTOOLBUTTONSTYLE_H
