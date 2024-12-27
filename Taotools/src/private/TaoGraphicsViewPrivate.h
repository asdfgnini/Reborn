#ifndef TAOGRAPHICSVIEWPRIVATE_H
#define TAOGRAPHICSVIEWPRIVATE_H

#include <QObject>

#include "stdafx.h"
class TaoGraphicsView;
class TaoGraphicsViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoGraphicsView)
    Q_PROPERTY_CREATE_D(qreal, MaxTransform);
    Q_PROPERTY_CREATE_D(qreal, MinTransform);

public:
    explicit TaoGraphicsViewPrivate(QObject* parent = nullptr);
    ~TaoGraphicsViewPrivate();

private:
    void _initStyle();
};

#endif // TaoGraphicsViewPRIVATE_H
