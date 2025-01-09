#ifndef TAOSCROLLPAGEPRIVATE_H
#define TAOSCROLLPAGEPRIVATE_H

#include <QMap>
#include <QObject>

#include "../../include/stdafx.h"
class TaoScrollPage;
class TaoScrollArea;
class QHBoxLayout;
class QVBoxLayout;
class TaoText;
class QStackedWidget;
class TaoBreadcrumbBar;
class TaoScrollPagePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoScrollPage)
    Q_PROPERTY_CREATE_D(QWidget*, CustomWidget)
public:
    explicit TaoScrollPagePrivate(QObject* parent = nullptr);
    ~TaoScrollPagePrivate();
    Q_INVOKABLE void onNavigationRouteBack(QVariantMap routeData);

private:
    QHBoxLayout* _pageTitleLayout{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QStackedWidget* _centralStackedWidget{nullptr};
    TaoBreadcrumbBar* _breadcrumbBar{nullptr};
    QMap<QString, int> _centralWidgetMap;
    int _navigationTargetIndex{0};
    int _pageTitleSpacing{0};
    bool _isGrabGesture{false};
    void _switchCentralStackIndex(int targetIndex, int lastIndex);
};

#endif // TAOSCROLLPAGEPRIVATE_H
