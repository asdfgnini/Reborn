#include "./inc/TaoListView.h"

#include <QMouseEvent>

#include "private/TaoListViewPrivate.h"
#include "./inc/TaoListViewStyle.h"
#include "./inc/TaoScrollBar.h"
TaoListView::TaoListView(QWidget* parent)
    : QListView(parent), d_ptr(new TaoListViewPrivate())
{
    Q_D(TaoListView);
    d->q_ptr = this;
    setObjectName("TaoListView");
    setStyleSheet("#TaoListView{background-color:transparent;}");
    d->_listViewStyle = new TaoListViewStyle(style());
    setStyle(d->_listViewStyle);
    setMouseTracking(true);
    setVerticalScrollBar(new TaoScrollBar(this));
    setHorizontalScrollBar(new TaoScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

TaoListView::~TaoListView()
{
}

void TaoListView::setItemHeight(int itemHeight)
{
    Q_D(TaoListView);
    if (itemHeight > 0)
    {
        d->_listViewStyle->setItemHeight(itemHeight);
        doItemsLayout();
    }
}

int TaoListView::getItemHeight() const
{
    Q_D(const TaoListView);
    return d->_listViewStyle->getItemHeight();
}

void TaoListView::setIsTransparent(bool isTransparent)
{
    Q_D(TaoListView);
    d->_listViewStyle->setIsTransparent(isTransparent);
    update();
}

bool TaoListView::getIsTransparent() const
{
    Q_D(const TaoListView);
    return d->_listViewStyle->getIsTransparent();
}
