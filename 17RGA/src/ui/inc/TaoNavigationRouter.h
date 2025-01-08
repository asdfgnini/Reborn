#ifndef TAONAVIGATIONROUTER_H
#define TAONAVIGATIONROUTER_H

#include <QObject>
#include <QVariantMap>

#include "../../include/Def.h"
#include "../../include/singleton.h"
#include "../../include/stdafx.h"

class TaoNavigationRouterPrivate;
class TAO_EXPORT TaoNavigationRouter : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(TaoNavigationRouter)
    Q_SINGLETON_CREATE_H(TaoNavigationRouter)
    Q_PROPERTY_CREATE_Q_H(int, MaxRouteCount)
private:
    explicit TaoNavigationRouter(QObject* parent = nullptr);
    ~TaoNavigationRouter();

Q_SIGNALS:
    Q_SIGNAL void navigationRouterStateChanged(bool state);

public:
    TaoNavigationRouterType::NavigationRouteType navigationRoute(QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData = {}, Qt::ConnectionType connectionType = Qt::AutoConnection);
    void navigationRouteBack();
};

#endif // TAONAVIGATIONROUTER_H
