#ifndef TAOPROGRESSBARSTYLE_H
#define TAOPROGRESSBARSTYLE_H

#include <QProxyStyle>

#include "../../include/Def.h"

class TaoProgressBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit TaoProgressBarStyle(QStyle* style = nullptr);
    ~TaoProgressBarStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOPROGRESSBARSTYLE_H
