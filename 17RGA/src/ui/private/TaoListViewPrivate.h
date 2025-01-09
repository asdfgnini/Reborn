#ifndef TAOLISTVIEWPRIVATE_H
#define TAOLISTVIEWPRIVATE_H

#include <QObject>

#include "../../include/stdafx.h"

class TaoListView;
class TaoListViewStyle;
class TaoListViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoListView)
public:
    explicit TaoListViewPrivate(QObject* parent = nullptr);
    ~TaoListViewPrivate();

private:
    TaoListViewStyle* _listViewStyle{nullptr};
};

#endif // TAOLISTVIEWPRIVATE_H
