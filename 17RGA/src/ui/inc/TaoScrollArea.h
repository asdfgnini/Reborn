#ifndef TAOSCROLLAREA_H
#define TAOSCROLLAREA_H

#include <QScrollArea>

#include "../../include/stdafx.h"


class TaoScrollAreaPrivate;
class TAO_EXPORT TaoScrollArea : public QScrollArea
{
    Q_OBJECT
    Q_Q_CREATE(TaoScrollArea)
public:
    explicit TaoScrollArea(QWidget* parent = nullptr);
    ~TaoScrollArea();

    void setIsGrabGesture(bool isEnable, qreal mousePressEventDTaoy = 0.5);

    void setIsOverShoot(Qt::Orientation orientation, bool isEnable);
    bool getIsOverShoot(Qt::Orientation orientation) const;

    void setIsAnimation(Qt::Orientation orientation, bool isAnimation);
    bool getIsAnimation(Qt::Orientation orientation) const;
};

#endif // TaoSCROLLAREA_H
