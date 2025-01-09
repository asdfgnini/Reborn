#ifndef TAOWINDOWPRIVATE_H
#define TAOWINDOWPRIVATE_H

#include <QLinearGradient>
#include <QMap>
#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoEvent;
class TaoWindow;
class TaoAppBar;
class TaoCentralStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class TaoThemeAnimationWidget;
class TaoWindowPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoWindow)
    Q_PROPERTY_CREATE_D(int, ThemeChangeTime)

public:
    explicit TaoWindowPrivate(QObject* parent = nullptr);
    ~TaoWindowPrivate();
    Q_SLOT void onThemeReadyChange();
    Q_SLOT void onThemeModeChanged(TaoThemeType::ThemeMode themeMode);

private:
    TaoThemeType::ThemeMode _themeMode;
    QImage _lightBaseImage;
    QImage _darkBaseImage;
    bool _isWindowClosing{false};

    bool _isInitFinished{false};
    TaoEvent* _focusEvent{nullptr};
    TaoCentralStackedWidget* _centerStackedWidget{nullptr};
    TaoAppBar* _appBar{nullptr};
    QHBoxLayout* _centerLayout{nullptr};
    int _contentsMargins{5};
    TaoThemeAnimationWidget* _animationWidget{nullptr};

    QMap<QString, int> _routeMap; // key__nodeKey title可以一致  value__stackIndex
    qreal _distance(QPoint point1, QPoint point2);
    void _resetWindowLayout(bool isAnimation);
};

#endif // TAOWINDOWPRIVATE_H
