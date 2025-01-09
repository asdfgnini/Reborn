#ifndef TAOSCROLLAREAPRIVATE_H
#define TAOSCROLLAREAPRIVATE_H

#include <QObject>
#include <QScrollBar>

#include "../../include/stdafx.h"
class TaoScrollBar;
class TaoScrollArea;
class TaoScrollAreaPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoScrollArea)
public:
    explicit TaoScrollAreaPrivate(QObject* parent = nullptr);
    ~TaoScrollAreaPrivate();
};

#endif // TAOSCROLLAREAPRIVATE_H
