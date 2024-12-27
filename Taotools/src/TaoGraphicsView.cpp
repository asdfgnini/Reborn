#include "TaoGraphicsView.h"

#include <QEvent>
#include <QScrollBar>
#include <QWheelEvent>

#include "private/TaoGraphicsViewPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(TaoGraphicsView, qreal, MaxTransform);
Q_PROPERTY_CREATE_Q_CPP(TaoGraphicsView, qreal, MinTransform);
TaoGraphicsView::TaoGraphicsView(QWidget* parent)
    : QGraphicsView(parent), d_ptr(new TaoGraphicsViewPrivate())
{
    Q_D(TaoGraphicsView);
    d->q_ptr = this;
    d->_pMaxTransform = 5;
    d->_pMinTransform = 0.15;
    d->_initStyle();
}

TaoGraphicsView::TaoGraphicsView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent), d_ptr(new TaoGraphicsViewPrivate())
{
    Q_D(TaoGraphicsView);
    d->q_ptr = this;
    d->_pMaxTransform = 5;
    d->_pMinTransform = 0.15;
    d->_initStyle();
}

TaoGraphicsView::~TaoGraphicsView()
{

}

void TaoGraphicsView::wheelEvent(QWheelEvent* event)
{
    Q_D(TaoGraphicsView);
    if (event->modifiers() == Qt::CTRL)
    {
        // 放大
        if ((event->angleDelta().y() > 0) && transform().m11() <= d->_pMaxTransform)
        {
            this->scale(1.1, 1.1);
        }
        else if ((event->angleDelta().y() < 0) && transform().m11() >= d->_pMinTransform)
        {
            this->scale(1.0 / 1.1, 1.0 / 1.1);
        }
        return;
    }
    QGraphicsView::wheelEvent(event);
}

void TaoGraphicsView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Alt)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    QGraphicsView::keyPressEvent(event);
}

void TaoGraphicsView::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Alt)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
    }
    QGraphicsView::keyReleaseEvent(event);
}
