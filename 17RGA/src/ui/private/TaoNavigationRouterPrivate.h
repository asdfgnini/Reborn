#ifndef TAONAVIGATIONROUTERPRIVATE_H
#define TAONAVIGATIONROUTERPRIVATE_H

#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "../../include/stdafx.h"

class TaoNavigationRouter;
class TaoNavigationRouterPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoNavigationRouter)
    Q_PROPERTY_CREATE_D(int, MaxRouteCount)
public:
    explicit TaoNavigationRouterPrivate(QObject* parent = nullptr);
    ~TaoNavigationRouterPrivate();

private:
    QQueue<QVariantMap> _routeQueue;
};

#endif // TAONAVIGATIONROUTERPRIVATE_H
