#ifndef TAOLINEEDITSTYLE_H
#define TAOLINEEDITSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"
class TaoLineEditStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TaoLineEditStyle(QStyle* style = nullptr);
    ~TaoLineEditStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOLINEEDITSTYLE_H
