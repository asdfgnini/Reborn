#include "./inc/TaoComboBoxView.h"

#include <QMouseEvent>

TaoComboBoxView::TaoComboBoxView(QWidget* parent)
    : QListView(parent)
{
}

TaoComboBoxView::~TaoComboBoxView()
{
}

void TaoComboBoxView::mousePressEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    if (index.isValid())
    {
        Q_EMIT itemPressed(index);
    }
    event->ignore();
}
