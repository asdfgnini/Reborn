#ifndef TAOBREADCRUMBBARDELEGATE_H
#define TAOBREADCRUMBBARDELEGATE_H

#include <QModelIndex>
#include <QStyledItemDelegate>

#include "../../include/Def.h"
class TaoBreadcrumbBarDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QModelIndex, PressIndex)
public:
    explicit TaoBreadcrumbBarDelegate(QObject* parent = nullptr);
    ~TaoBreadcrumbBarDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOBREADCRUMBBARDELEGATE_H
