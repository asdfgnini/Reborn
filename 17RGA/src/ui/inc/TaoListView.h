#ifndef TAOLISTVIEW_H
#define TAOLISTVIEW_H

#include <QListView>

#include "../../include/stdafx.h"

class TaoListViewPrivate;
class TAO_EXPORT TaoListView : public QListView
{
    Q_OBJECT
    Q_Q_CREATE(TaoListView)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
public:
    explicit TaoListView(QWidget* parent = nullptr);
    ~TaoListView();
};

#endif // TAOLISTVIEW_H
