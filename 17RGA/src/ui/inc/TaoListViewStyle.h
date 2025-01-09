#ifndef TAOLISTVIEWSTYLE_H
#define TAOLISTVIEWSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"
class TaoListViewStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, ItemHeight)
    Q_PROPERTY_CREATE(bool, IsTransparent)
public:
    explicit TaoListViewStyle(QStyle* style = nullptr);
    ~TaoListViewStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
    int _leftPadding{11};
};

#endif // TAOLISTVIEWSTYLE_H
