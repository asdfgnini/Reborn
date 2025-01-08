#ifndef TAOTHEMEPRIVATE_H
#define TAOTHEMEPRIVATE_H

#include <QColor>
#include <QMap>
#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"
class TaoTheme;
class TaoThemePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoTheme)
public:
    explicit TaoThemePrivate(QObject* parent = nullptr);
    ~TaoThemePrivate();

private:
    TaoThemeType::ThemeMode _themeMode{TaoThemeType::Light};
    QColor _lightThemeColorList[40];
    QColor _darkThemeColorList[40];
    void _initThemeColor();
};

#endif // TAOTHEMEPRIVATE_H
