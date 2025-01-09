#ifndef TAOSCROLLPAGEAREAPRIVATE_H
#define TAOSCROLLPAGEAREAPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"
class TaoScrollPageArea;
class TaoScrollPageAreaPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoScrollPageArea)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit TaoScrollPageAreaPrivate(QObject* parent = nullptr);
    ~TaoScrollPageAreaPrivate();

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOSCROLLPAGEAREAPRIVATE_H
