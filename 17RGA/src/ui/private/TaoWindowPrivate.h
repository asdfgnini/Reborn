#ifndef TAOWINDOWPRIVATE_H
#define TAOWINDOWPRIVATE_H

#include <QLinearGradient>
#include <QMap>
#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoEvent;
class TaoWindow;
class TaoNavigationBar;
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
    Q_PROPERTY_CREATE_D(TaoNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)

public:
    explicit TaoWindowPrivate(QObject* parent = nullptr);
    ~TaoWindowPrivate();
    Q_SLOT void onNavigationButtonClicked();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeReadyChange();
    Q_SLOT void onDisplayModeChanged();
    Q_SLOT void onThemeModeChanged(TaoThemeType::ThemeMode themeMode);
    Q_SLOT void onNavigationNodeClicked(TaoNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SLOT void onNavigationNodeAdded(TaoNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);

private:
    TaoThemeType::ThemeMode _themeMode;
    QImage _lightBaseImage;
    QImage _darkBaseImage;
    bool _isWindowClosing{false};

    bool _isInitFinished{false};
    TaoEvent* _focusEvent{nullptr};
    TaoNavigationBar* _navigationBar{nullptr};
    TaoCentralStackedWidget* _centerStackedWidget{nullptr};
    TaoAppBar* _appBar{nullptr};
    QHBoxLayout* _centerLayout{nullptr};
    int _contentsMargins{5};
    bool _isNavigationDisplayModeChanged{false};
    bool _isNavigationEnable{true};
    bool _isNavigationBarExpanded{false};
    bool _isWMClickedAnimationFinished{true};
    TaoThemeAnimationWidget* _animationWidget{nullptr};

    TaoNavigationType::NavigationDisplayMode _currentNavigationBarDisplayMode{TaoNavigationType::Maximal};

    QMap<QString, int> _routeMap; // key__nodeKey title可以一致  value__stackIndex
    int _navigationTargetIndex{0};
    qreal _distance(QPoint point1, QPoint point2);
    void _resetWindowLayout(bool isAnimation);
    void _doNavigationDisplayModeChange();
};

#endif // TAOWINDOWPRIVATE_H
