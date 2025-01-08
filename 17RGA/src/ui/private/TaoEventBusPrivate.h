#ifndef TAOEVENTBUSPRIVATE_H
#define TAOEVENTBUSPRIVATE_H

#include <QMap>
#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoEvent;
class TaoEventPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoEvent)
    Q_PROPERTY_CREATE_D(QString, EventName);
    Q_PROPERTY_CREATE_D(QString, FunctionName);
    Q_PROPERTY_CREATE_D(Qt::ConnectionType, ConnectionType);

public:
    explicit TaoEventPrivate(QObject* parent = nullptr);
    ~TaoEventPrivate();
};

class TaoEventBus;
class TaoEventBusPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoEventBus)
public:
    explicit TaoEventBusPrivate(QObject* parent = nullptr);
    ~TaoEventBusPrivate();
    TaoEventBusType::EventBusReturnType registerEvent(TaoEvent* event);
    void unRegisterEvent(TaoEvent* event);

private:
    QMap<QString, QList<TaoEvent*>> _eventMap;
};

#endif // TAOEVENTBUSPRIVATE_H
