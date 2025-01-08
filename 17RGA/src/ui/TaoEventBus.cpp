#include "./inc/TaoEventBus.h"

#include <QVariant>

#include "./private/TaoEventBusPrivate.h"

Q_SINGLETON_CREATE_CPP(TaoEventBus);
Q_PROPERTY_CREATE_Q_CPP(TaoEvent, QString, EventName);
Q_PROPERTY_CREATE_Q_CPP(TaoEvent, QString, FunctionName);
Q_PROPERTY_CREATE_Q_CPP(TaoEvent, Qt::ConnectionType, ConnectionType);
TaoEvent::TaoEvent(QObject* parent)
    : QObject{parent}, d_ptr(new TaoEventPrivate())
{
    Q_D(TaoEvent);
    d->q_ptr = this;
    d->_pConnectionType = Qt::AutoConnection;
    d->_pFunctionName = "";
    d->_pEventName = "";
}

TaoEvent::TaoEvent(QString eventName, QString functionName, QObject* parent)
    : QObject{parent}, d_ptr(new TaoEventPrivate())
{
    Q_D(TaoEvent);
    d->q_ptr = this;
    d->_pConnectionType = Qt::AutoConnection;
    d->_pEventName = eventName;
    d->_pFunctionName = functionName;
}

TaoEventBusType::EventBusReturnType TaoEvent::registerAndInit()
{
    return TaoEventBus::getInstance()->d_ptr->registerEvent(this);
}

TaoEvent::~TaoEvent()
{
    TaoEventBus::getInstance()->d_ptr->unRegisterEvent(this);
}

TaoEventBus::TaoEventBus(QObject* parent)
    : QObject{parent}, d_ptr(new TaoEventBusPrivate())
{
    Q_D(TaoEventBus);
    d->q_ptr = this;
}

TaoEventBus::~TaoEventBus()
{
}

TaoEventBusType::EventBusReturnType TaoEventBus::post(const QString& eventName, const QVariantMap& data)
{
    Q_D(TaoEventBus);
    if (eventName.isEmpty())
    {
        return TaoEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if (d->_eventMap.contains(eventName))
    {
        QList<TaoEvent*> eventList = d->_eventMap.value(eventName);
        foreach (auto event, eventList)
        {
            if (event->parent())
            {
                QMetaObject::invokeMethod(event->parent(), event->getFunctionName().toLocal8Bit().constData(), event->getConnectionType(), Q_ARG(QVariantMap, data));
            }
        }
    }
    return TaoEventBusType::EventBusReturnType::Success;
}

QStringList TaoEventBus::getRegisteredEventsName() const
{
    Q_D(const TaoEventBus);
    if (d->_eventMap.count() == 0)
    {
        return QStringList();
    }
    QStringList eventsNameList = d->_eventMap.keys();
    return eventsNameList;
}
