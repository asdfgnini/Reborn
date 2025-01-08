#ifndef TAOMENUPRIVATE_H
#define TAOMENUPRIVATE_H

#include <QObject>
#include <QPixmap>
#include <QPoint>

#include "../../include/stdafx.h"


class TaoMenu;
class TaoMenuStyle;
class TaoMenuPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoMenu)
    Q_PROPERTY_CREATE(int, AnimationImagePosY)
public:
    explicit TaoMenuPrivate(QObject* parent = nullptr);
    ~TaoMenuPrivate();

private:
    QPixmap _animationPix;
    bool _isCloseAnimation{false};
    QPoint _mousePressPoint;
    TaoMenuStyle* _menuStyle{nullptr};
};

#endif // TAOMENUPRIVATE_H
