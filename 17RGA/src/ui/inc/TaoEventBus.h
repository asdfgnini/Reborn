#ifndef TAOEVENTBUS_H
#define TAOEVENTBUS_H

#include <QObject>
#include <QVariantMap>

#include "../../include/Def.h"
#include "../../include/stdafx.h"
#include "../../include/singleton.h"
class TaoEventPrivate;
class TAO_EXPORT TaoEvent : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(TaoEvent)
    Q_PROPERTY_CREATE_Q_H(QString, EventName);
    Q_PROPERTY_CREATE_Q_H(QString, FunctionName);
    Q_PROPERTY_CREATE_Q_H(Qt::ConnectionType, ConnectionType);

public:
    explicit TaoEvent(QObject* parent = nullptr);
    explicit TaoEvent(QString eventName, QString functionName, QObject* parent = nullptr);
    ~TaoEvent();
    TaoEventBusType::EventBusReturnType registerAndInit();
};

class TaoEventBusPrivate;
class TAO_EXPORT TaoEventBus : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(TaoEventBus)
    Q_SINGLETON_CREATE_H(TaoEventBus);

private:
    explicit TaoEventBus(QObject* parent = nullptr);
    ~TaoEventBus();

public:
    TaoEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});
    QStringList getRegisteredEventsName() const;

private:
    friend class TaoEvent;
};

#endif // TAOEVENTBUS_H
