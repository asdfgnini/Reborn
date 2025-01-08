#ifndef TAOSUGGESTDELEGATE_H
#define TAOSUGGESTDELEGATE_H

#include <QStyledItemDelegate>

#include "../../include/Def.h"

class TaoSuggestDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TaoSuggestDelegate(QObject* parent = nullptr);
    ~TaoSuggestDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOSUGGESTDELEGATE_H
