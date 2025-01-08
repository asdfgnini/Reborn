#ifndef TAODOCKWIDGETPRIVATE_H
#define TAODOCKWIDGETPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoDockWidget;
class TaoDockWidgetTitleBar;
class TaoDockWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoDockWidget)
public:
    explicit TaoDockWidgetPrivate(QObject* parent = nullptr);
    ~TaoDockWidgetPrivate();
    Q_SLOT void onThemeModeChanged(TaoThemeType::ThemeMode themeMode);

private:
    qint64 _currentWinID{0};
    int _margins{8};
    TaoThemeType::ThemeMode _themeMode;
    bool _isEnableMica;
    int _shadowBorderWidth{6};
    TaoDockWidgetTitleBar* _titleBar{nullptr};
};

#endif // TAODOCKWIDGETPRIVATE_H
