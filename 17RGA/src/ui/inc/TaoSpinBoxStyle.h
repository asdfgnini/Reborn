#ifndef TAOSPINBOXSTYLE_H
#define TAOSPINBOXSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"
class TaoSpinBoxStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TaoSpinBoxStyle(QStyle* style = nullptr);
    ~TaoSpinBoxStyle();
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOSPINBOXSTYLE_H
