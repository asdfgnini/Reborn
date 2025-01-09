#ifndef TAOBREADCRUMBBARPRIVATE_H
#define TAOBREADCRUMBBARPRIVATE_H

#include <QObject>

#include "../../include/stdafx.h"
class TaoBaseListView;
class TaoBreadcrumbBarModel;
class TaoBreadcrumbBarDelegate;

class TaoBreadcrumbBar;
class TaoBreadcrumbBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoBreadcrumbBar)
    Q_PROPERTY_CREATE_D(int, TextPixelSize)
    Q_PROPERTY_CREATE_D(bool, IsAutoRemove)
public:
    explicit TaoBreadcrumbBarPrivate(QObject* parent = nullptr);
    ~TaoBreadcrumbBarPrivate();

private:
    TaoBreadcrumbBarModel* _listModel{nullptr};
    TaoBreadcrumbBarDelegate* _listDelegate{nullptr};
    TaoBaseListView* _listView{nullptr};
};

#endif // TAOBREADCRUMBBARPRIVATE_H
