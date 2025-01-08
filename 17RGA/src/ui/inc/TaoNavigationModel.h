#ifndef TAONAVIGATIONMODEL_H
#define TAONAVIGATIONMODEL_H

#include <QAbstractItemModel>
#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoNavigationNode;
class TaoNavigationModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PRIVATE_CREATE(TaoNavigationNode*, SelectedNode)
    Q_PRIVATE_CREATE(TaoNavigationNode*, SelectedExpandedNode)

public:
    explicit TaoNavigationModel(QObject* parent = nullptr);
    ~TaoNavigationModel();
    QModelIndex parent(const QModelIndex& child) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    TaoNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, TaoIconType::IconName awesome);
    TaoNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, TaoIconType::IconName awesome);
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, TaoIconType::IconName awesome);
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, TaoIconType::IconName awesome);
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, int keyPoints, TaoIconType::IconName awesome);
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, int keyPoints, TaoIconType::IconName awesome);

    TaoNavigationNode* getNavigationNode(QString nodeKey) const;
    QList<TaoNavigationNode*> getRootExpanderNodes() const;
    QList<TaoNavigationNode*> getRootExpandedNodes() const;

private:
    mutable QMap<QString, TaoNavigationNode*> _nodesMap;
    TaoNavigationNode* _rootNode{nullptr};
};

#endif // TAONAVIGATIONMODEL_H
