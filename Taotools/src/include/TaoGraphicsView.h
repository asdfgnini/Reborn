#ifndef TAOGRAPHICSVIEW_H
#define TAOGRAPHICSVIEW_H

#include <QGraphicsView>

#include "stdafx.h"

class TaoGraphicsViewPrivate;
class TAO_EXPORT TaoGraphicsView : public QGraphicsView
{
    Q_OBJECT
    Q_Q_CREATE(TaoGraphicsView)
    Q_PROPERTY_CREATE_Q_H(qreal, MaxTransform);
    Q_PROPERTY_CREATE_Q_H(qreal, MinTransform);

public:
    explicit TaoGraphicsView(QWidget* parent = nullptr);
    explicit TaoGraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);
    ~TaoGraphicsView();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
};

#endif // ELAGRAPHICSVIEW_H
