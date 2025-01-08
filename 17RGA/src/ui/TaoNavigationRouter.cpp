#include "./inc/TaoNavigationRouter.h"

#include "private/TaoNavigationRouterPrivate.h"

Q_SINGLETON_CREATE_CPP(TaoNavigationRouter)
Q_PROPERTY_CREATE_Q_CPP(TaoNavigationRouter, int, MaxRouteCount)
TaoNavigationRouter::TaoNavigationRouter(QObject* parent)
    : QObject{parent}, d_ptr(new TaoNavigationRouterPrivate())
{
    Q_D(TaoNavigationRouter);
    d->q_ptr = this;
    d->_pMaxRouteCount = 25;
}

TaoNavigationRouter::~TaoNavigationRouter()
{
}

TaoNavigationRouterType::NavigationRouteType TaoNavigationRouter::navigationRoute(QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData, Qt::ConnectionType connectionType)
{
    Q_D(TaoNavigationRouter);
    if (!routeObject)
    {
        return TaoNavigationRouterType::ObjectInvalid;
    }
    if (routeFunctionName.isEmpty())
    {
        return TaoNavigationRouterType::FunctionNameInvalid;
    }
    if (d->_routeQueue.isEmpty())
    {
        Q_EMIT navigationRouterStateChanged(true);
    }
    else
    {
        if (d->_routeQueue.count() >= d->_pMaxRouteCount)
        {
            d->_routeQueue.dequeue();
        }
    }
    QVariantMap saveData;
    saveData.insert("TaoRouteObject", QVariant::fromValue<QObject*>(routeObject));
    saveData.insert("TaoRouteFunctionName", routeFunctionName);
    saveData.insert("TaoRouteData", routeData);
    saveData.insert("TaoRouteConnectionType", QVariant::fromValue<Qt::ConnectionType>(connectionType));
    d->_routeQueue.enqueue(saveData);
    return TaoNavigationRouterType::Success;
}

void TaoNavigationRouter::navigationRouteBack()
{
    Q_D(TaoNavigationRouter);
    int routeQueneCount = d->_routeQueue.count();
    if (routeQueneCount > 0)
    {
        if (routeQueneCount == 1)
        {
            Q_EMIT navigationRouterStateChanged(false);
        }
        QVariantMap routeSaveData = d->_routeQueue.last();
        d->_routeQueue.removeLast();
        QObject* routeObject = routeSaveData.value("TaoRouteObject").value<QObject*>();
        QString routeFunctionName = routeSaveData.value("TaoRouteFunctionName").toString();
        QVariantMap routeData = routeSaveData.value("TaoRouteData").toMap();
        Qt::ConnectionType connectionType = routeSaveData.value("TaoRouteConnectionType").value<Qt::ConnectionType>();
        QMetaObject::invokeMethod(routeObject, routeFunctionName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
    }
}
