#include "./inc/TaoFooterModel.h"

#include <QUuid>

#include "./inc/TaoNavigationNode.h"

TaoFooterModel::TaoFooterModel(QObject* parent)
    : QAbstractListModel{parent}
{
    _pSelectedNode = nullptr;
}

TaoFooterModel::~TaoFooterModel()
{
    qDeleteAll(_footerNodeList);
}

int TaoFooterModel::rowCount(const QModelIndex& parent) const
{
    return _footerNodeList.count();
}

QVariant TaoFooterModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::UserRole)
    {
        if (index.row() < _footerNodeList.count())
        {
            return QVariant::fromValue(_footerNodeList[index.row()]);
        }
    }
    return QVariant();
}

TaoNavigationType::NodeOperateReturnType TaoFooterModel::addFooterNode(QString footerTitle, QString& footerKey, bool isHasFooterPage, int keyPoints, TaoIconType::IconName awesome)
{
    if (_footerNodeList.count() >= 3)
    {
        return TaoNavigationType::FooterUpperLimit;
    }
    TaoNavigationNode* node = new TaoNavigationNode(footerTitle);
    node->setKeyPoints(keyPoints);
    node->setIsFooterNode(true);
    node->setIsHasFooterPage(isHasFooterPage);
    node->setAwesome(awesome);
    footerKey = node->getNodeKey();
    beginResetModel();
    _footerNodeList.append(node);
    endResetModel();
    node->setModelIndex(this->index(_footerNodeList.count() - 1));
    return TaoNavigationType::Success;
}

int TaoFooterModel::getFooterNodeCount() const
{
    return _footerNodeList.count();
}

TaoNavigationNode* TaoFooterModel::getNavigationNode(QString footerKey)
{
    for (auto node : _footerNodeList)
    {
        if (node->getNodeKey() == footerKey)
        {
            return node;
        }
    }
    return nullptr;
}
