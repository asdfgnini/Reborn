#ifndef TAOPLAINTEXTEDITSTYLE_H
#define TAOPLAINTEXTEDITSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"

class TaoPlainTextEditStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit TaoPlainTextEditStyle(QStyle* style = nullptr);
    ~TaoPlainTextEditStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOPLAINTEXTEDITSTYLE_H
