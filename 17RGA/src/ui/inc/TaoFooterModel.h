#ifndef TAOFOOTERMODEL_H
#define TAOFOOTERMODEL_H

#include <QAbstractListModel>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoNavigationNode;
class TaoFooterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PRIVATE_CREATE(TaoNavigationNode*, SelectedNode)
public:
    explicit TaoFooterModel(QObject* parent = nullptr);
    ~TaoFooterModel();
    TaoNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None);
    int getFooterNodeCount() const;
    TaoNavigationNode* getNavigationNode(QString footerKey);

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QList<TaoNavigationNode*> _footerNodeList;
};

#endif // TAOFOOTERMODEL_H
