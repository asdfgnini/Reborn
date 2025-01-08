#include "TaoEventBusPrivate.h"

#include "../inc/TaoEventBus.h"
TaoEventPrivate::TaoEventPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoEventPrivate::~TaoEventPrivate()
{
}

TaoEventBusPrivate::TaoEventBusPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoEventBusPrivate::~TaoEventBusPrivate()
{
}

TaoEventBusType::EventBusReturnType TaoEventBusPrivate::registerEvent(TaoEvent* event)
{
    if (!event)
    {
        return TaoEventBusType::EventBusReturnType::EventInvalid;
    }
    if (event->getEventName().isEmpty())
    {
        return TaoEventBusType::EventBusReturnType::EventNameInvalid;
    }
    if (_eventMap.contains(event->getEventName()))
    {
        QList<TaoEvent*> eventList = _eventMap.value(event->getEventName());
        if (eventList.contains(event))
        {
            return TaoEventBusType::EventBusReturnType::EventInvalid;
        }
        eventList.append(event);
        _eventMap[event->getEventName()] = eventList;
    }
    else
    {
        QList<TaoEvent*> eventList;
        eventList.append(event);
        _eventMap.insert(event->getEventName(), eventList);
    }
    return TaoEventBusType::EventBusReturnType::Success;
}

void TaoEventBusPrivate::unRegisterEvent(TaoEvent* event)
{
    if (!event)
    {
        return;
    }
    if (event->getEventName().isEmpty())
    {
        return;
    }
    if (_eventMap.contains(event->getEventName()))
    {
        if (_eventMap[event->getEventName()].count() == 1)
        {
            _eventMap.remove(event->getEventName());
        }
        else
        {
            QList<TaoEvent*> eventList = _eventMap.value(event->getEventName());
            eventList.removeOne(event);
            _eventMap[event->getEventName()] = eventList;
        }
    }
}
