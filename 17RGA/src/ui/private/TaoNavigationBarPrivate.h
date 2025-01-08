#ifndef TAONAVIGATIONBARPRIVATE_H
#define TAONAVIGATIONBARPRIVATE_H

#include <QMap>
#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class QLayout;
class TaoMenu;
class QVBoxLayout;
class QHBoxLayout;
class QLinearGradient;

class TaoNavigationBar;
class TaoNavigationNode;
class TaoNavigationModel;
class TaoNavigationView;
class TaoNavigationDelegate;
class TaoSuggestBox;
class TaoInteractiveCard;

class TaoBaseListView;
class TaoFooterModel;
class TaoFooterDelegate;
class TaoIconButton;
class TaoToolButton;
class TaoNavigationBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoNavigationBar)
    Q_PROPERTY_CREATE_D(bool, IsTransparent)
    Q_PROPERTY_CREATE(int, NavigationViewWidth);

public:
    explicit TaoNavigationBarPrivate(QObject* parent = nullptr);
    ~TaoNavigationBarPrivate();
    Q_SLOT void onNavigationButtonClicked();
    Q_SLOT void onNavigationOpenNewWindow(QString nodeKey);

    Q_INVOKABLE void onNavigationRouteBack(QVariantMap routeData);

    //核心跳转逻辑
    void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true);
    void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true);

private:
    TaoThemeType::ThemeMode _themeMode;
    QMap<QString, const QMetaObject*> _pageMetaMap;
    QMap<TaoNavigationNode*, TaoMenu*> _compactMenuMap;
    QVBoxLayout* _navigationButtonLayout{nullptr};
    QHBoxLayout* _navigationSuggestLayout{nullptr};
    QVBoxLayout* _userButtonLayout{nullptr};

    TaoIconButton* _userButton{nullptr};
    TaoToolButton* _searchButton{nullptr};
    TaoToolButton* _navigationButton{nullptr};
    TaoNavigationModel* _navigationModel{nullptr};
    TaoNavigationView* _navigationView{nullptr};
    TaoBaseListView* _footerView{nullptr};
    TaoFooterModel* _footerModel{nullptr};
    TaoFooterDelegate* _footerDelegate{nullptr};
    TaoSuggestBox* _navigationSuggestBox{nullptr};
    TaoInteractiveCard* _userCard{nullptr};
    bool _isShowUserCard{true};

    QList<TaoNavigationNode*> _lastExpandedNodesList;

    TaoNavigationType::NavigationDisplayMode _currentDisplayMode{TaoNavigationType::NavigationDisplayMode::Maximal};
    void _resetNodeSelected();
    void _expandSelectedNodeParent();
    void _initNodeModelIndex(const QModelIndex& parentIndex);

    void _addStackedPage(QWidget* page, QString pageKey);
    void _addFooterPage(QWidget* page, QString footKey);

    void _raiseNavigationBar();

    void _doComponentAnimation(TaoNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _handleNavigationExpandState(bool isSave);
    void _handleMaximalToCompactLayout();
    void _handleCompactToMaximalLayout();
    void _resetLayout();

    void _doNavigationBarWidthAnimation(TaoNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _doNavigationViewWidthAnimation(bool isAnimation);
    void _doNavigationButtonAnimation(bool isCompact, bool isAnimation);
    void _doSearchButtonAnimation(bool isCompact, bool isAnimation);
    void _doUserButtonAnimation(bool isCompact, bool isAnimation);
};

#endif // TAONAVIGATIONBARPRIVATE_H
