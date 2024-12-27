#include "TaoGraphicsViewPrivate.h"

#include "TaoGraphicsView.h"
// #include "ElaScrollBar.h"

TaoGraphicsViewPrivate::TaoGraphicsViewPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoGraphicsViewPrivate::~TaoGraphicsViewPrivate()
{

}

void TaoGraphicsViewPrivate::_initStyle()
{
    Q_Q(TaoGraphicsView);
    q->setDragMode(QGraphicsView::RubberBandDrag);
    q->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    q->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    q->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    q->setMouseTracking(true);
    q->setObjectName("TaoGraphicsView");
    q->setStyleSheet("#TaoGraphicsView{background-color:white;border:0px;}");
    // q->setVerticalScrollBar(new ElaScrollBar(q));
    // q->setHorizontalScrollBar(new ElaScrollBar(q));
}
