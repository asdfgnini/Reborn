#ifndef TAOTHEME_H
#define TAOTHEME_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"
#include "../../include/singleton.h"

#define tTheme TaoTheme::getInstance()
#define TaoThemeColor(themeMode, themeColor) tTheme->getThemeColor(themeMode, TaoThemeType::themeColor)
class QPainter;
class TaoThemePrivate;
class TAO_EXPORT TaoTheme : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(TaoTheme)
    Q_SINGLETON_CREATE_H(TaoTheme)
private:
    explicit TaoTheme(QObject* parent = nullptr);
    ~TaoTheme();

public:
    void setThemeMode(TaoThemeType::ThemeMode themeMode);
    TaoThemeType::ThemeMode getThemeMode() const;

    void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);

    void setThemeColor(TaoThemeType::ThemeMode themeMode, TaoThemeType::ThemeColor themeColor, QColor newColor);
    const QColor& getThemeColor(TaoThemeType::ThemeMode themeMode, TaoThemeType::ThemeColor themeColor);
Q_SIGNALS:
    Q_SIGNAL void themeModeChanged(TaoThemeType::ThemeMode themeMode);
};

#endif // TAOTHEME_H
