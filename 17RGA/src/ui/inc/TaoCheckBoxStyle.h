#ifndef TAOCHECKBOXSTYLE_H
#define TAOCHECKBOXSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"

class TaoCheckBoxStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(int, CheckIndicatorWidth)
public:
    explicit TaoCheckBoxStyle(QStyle* style = nullptr);
    ~TaoCheckBoxStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOCHECKBOXSTYLE_H
