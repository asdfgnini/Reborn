#include "./inc/TaoBaseListView.h"

#include <QMouseEvent>

#include "./inc/TaoScrollBar.h"
TaoBaseListView::TaoBaseListView(QWidget* parent)
    : QListView(parent)
{
    setObjectName("TaoBaseListView");
    setStyleSheet(
        "TaoBaseListView{background-color: transparent;border:0px;}"
        "TaoBaseListView::item{border:none;}");
    setAutoScroll(false);
    setFocusPolicy(Qt::NoFocus);
    setVerticalScrollBar(new TaoScrollBar(this));
    setHorizontalScrollBar(new TaoScrollBar(this));
    setSelectionMode(QAbstractItemView::NoSelection);
    setMouseTracking(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

TaoBaseListView::~TaoBaseListView()
{
}

void TaoBaseListView::mousePressEvent(QMouseEvent* event)
{
    Q_EMIT mousePress(indexAt(event->pos()));
    QListView::mousePressEvent(event);
}

void TaoBaseListView::mouseReleaseEvent(QMouseEvent* event)
{
    Q_EMIT mouseRelease(indexAt(event->pos()));
    QListView::mouseReleaseEvent(event);
}

void TaoBaseListView::mouseDoubleClickEvent(QMouseEvent* event)
{
    Q_EMIT mouseDoubleClick(indexAt(event->pos()));
    QListView::mouseDoubleClickEvent(event);
}
