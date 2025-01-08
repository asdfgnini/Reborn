#ifndef TAOFOOTERDELEGATE_H
#define TAOFOOTERDELEGATE_H

#include <QModelIndex>
#include <QStyledItemDelegate>

#include "../../include/Def.h"

class TaoNavigationNode;
class QPropertyAnimation;
class TaoBaseListView;
class TaoFooterDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    Q_PRIVATE_CREATE(TaoBaseListView*, TaoListView);
    Q_PRIVATE_CREATE(QModelIndex, PressIndex);

public:
    explicit TaoFooterDelegate(QObject* parent = nullptr);
    ~TaoFooterDelegate();
    void navigationNodeStateChange(QVariantMap data);

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    int _leftPadding{11};
    int _iconAreaWidth{40};
    int _textRightSpacing{3};
    int _indicatorIconAreaWidth{24};
    qreal _lastSelectMarkTop{10};
    qreal _lastSelectMarkBottom{10};
    qreal _selectMarkTop{10};
    qreal _selectMarkBottom{10};
    bool _isSelectMarkDisplay{true};
    TaoNavigationNode* _lastSelectedNode{nullptr};
    TaoThemeType::ThemeMode _themeMode;
    QPropertyAnimation* _lastSelectMarkTopAnimation{nullptr};
    QPropertyAnimation* _lastSelectMarkBottomAnimation{nullptr};
    QPropertyAnimation* _selectMarkTopAnimation{nullptr};
    QPropertyAnimation* _selectMarkBottomAnimation{nullptr};
    bool _compareItemY(TaoNavigationNode* node1, TaoNavigationNode* node2);
};

#endif // TAOFOOTERDELEGATE_H
