#include "./inc/TaoNavigationModel.h"

#include <QIcon>
#include <QJsonObject>

#include "./inc/TaoNavigationNode.h"

TaoNavigationModel::TaoNavigationModel(QObject* parent)
    : QAbstractItemModel{parent}
{
    _rootNode = new TaoNavigationNode("root");
    _rootNode->setIsRootNode(true);
    _rootNode->setIsExpanderNode(true);
    _pSelectedNode = nullptr;
    _pSelectedExpandedNode = nullptr;
}

TaoNavigationModel::~TaoNavigationModel()
{
    delete _rootNode;
}

QModelIndex TaoNavigationModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }
    TaoNavigationNode* childNode = static_cast<TaoNavigationNode*>(child.internalPointer());
    TaoNavigationNode* parentNode = childNode->getParentNode();
    if (parentNode == _rootNode)
    {
        return QModelIndex();
    }
    else if (parentNode == nullptr)
    {
        return QModelIndex();
    }
    return createIndex(parentNode->getRow(), 0, parentNode);
}

QModelIndex TaoNavigationModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }
    TaoNavigationNode* parentNode;
    if (!parent.isValid())
    {
        parentNode = _rootNode;
    }
    else
    {
        parentNode = static_cast<TaoNavigationNode*>(parent.internalPointer());
    }
    TaoNavigationNode* childNode = nullptr;
    if (parentNode->getChildrenNodes().count() > row)
    {
        childNode = parentNode->getChildrenNodes().at(row);
    }
    if (childNode)
    {
        return createIndex(row, column, childNode);
    }
    return QModelIndex();
}

int TaoNavigationModel::rowCount(const QModelIndex& parent) const
{
    TaoNavigationNode* parentNode;
    if (parent.column() > 0)
    {
        return 0;
    }
    if (!parent.isValid())
    {
        parentNode = _rootNode;
    }
    else
    {
        parentNode = static_cast<TaoNavigationNode*>(parent.internalPointer());
    }
    return parentNode->getChildrenNodes().count();
};

int TaoNavigationModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant TaoNavigationModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(index)
    Q_UNUSED(role)
    return QVariant();
}

TaoNavigationType::NodeOperateReturnType TaoNavigationModel::addExpanderNode(QString expanderTitle, QString& expanderKey, TaoIconType::IconName awesome)
{
    TaoNavigationNode* node = new TaoNavigationNode(expanderTitle, _rootNode);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    expanderKey = node->getNodeKey();
    return TaoNavigationType::NodeOperateReturnType::Success;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationModel::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, TaoIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    TaoNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 10)
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    TaoNavigationNode* node = new TaoNavigationNode(expanderTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setIsExpanderNode(true);
    node->setAwesome(awesome);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    expanderKey = node->getNodeKey();
    return TaoNavigationType::NodeOperateReturnType::Success;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationModel::addPageNode(QString pageTitle, QString& pageKey, TaoIconType::IconName awesome)
{
    TaoNavigationNode* node = new TaoNavigationNode(pageTitle, _rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    pageKey = node->getNodeKey();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return TaoNavigationType::NodeOperateReturnType::Success;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationModel::addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, TaoIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    TaoNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 10)
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    TaoNavigationNode* node = new TaoNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(awesome);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    pageKey = node->getNodeKey();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return TaoNavigationType::NodeOperateReturnType::Success;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationModel::addPageNode(QString pageTitle, QString& pageKey, int keyPoints, TaoIconType::IconName awesome)
{
    TaoNavigationNode* node = new TaoNavigationNode(pageTitle, _rootNode);
    node->setAwesome(awesome);
    node->setDepth(1);
    node->setIsVisible(true);
    node->setKeyPoints(keyPoints);
    beginInsertRows(QModelIndex(), _rootNode->getChildrenNodes().count(), _rootNode->getChildrenNodes().count());
    _rootNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    pageKey = node->getNodeKey();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return TaoNavigationType::NodeOperateReturnType::Success;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationModel::addPageNode(QString pageTitle, QString& pageKey, QString targetExpanderKey, int keyPoints, TaoIconType::IconName awesome)
{
    if (!_nodesMap.contains(targetExpanderKey))
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeInvalid;
    }
    TaoNavigationNode* parentNode = _nodesMap.value(targetExpanderKey);
    if (!parentNode->getIsExpanderNode())
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeTypeError;
    }
    if (parentNode->getDepth() > 10)
    {
        return TaoNavigationType::NodeOperateReturnType::TargetNodeDepthLimit;
    }
    TaoNavigationNode* node = new TaoNavigationNode(pageTitle, parentNode);
    node->setDepth(parentNode->getDepth() + 1);
    node->setAwesome(awesome);
    node->setKeyPoints(keyPoints);
    if (parentNode->getIsVisible() && parentNode->getIsExpanded())
    {
        node->setIsVisible(true);
    }
    beginInsertRows(parentNode->getModelIndex(), parentNode->getChildrenNodes().count(), parentNode->getChildrenNodes().count());
    parentNode->appendChildNode(node);
    _nodesMap.insert(node->getNodeKey(), node);
    endInsertRows();
    pageKey = node->getNodeKey();
    if (!_pSelectedNode)
    {
        _pSelectedNode = node;
    }
    return TaoNavigationType::NodeOperateReturnType::Success;
}

TaoNavigationNode* TaoNavigationModel::getNavigationNode(QString nodeKey) const
{
    if (_nodesMap.contains(nodeKey))
    {
        return _nodesMap.value(nodeKey);
    }
    return nullptr;
}

QList<TaoNavigationNode*> TaoNavigationModel::getRootExpanderNodes() const
{
    QList<TaoNavigationNode*> expandedNodeList;
    for (auto node : _rootNode->getChildrenNodes())
    {
        if (node->getIsExpanderNode())
        {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}

QList<TaoNavigationNode*> TaoNavigationModel::getRootExpandedNodes() const
{
    QList<TaoNavigationNode*> expandedNodeList;
    for (auto node : _rootNode->getChildrenNodes())
    {
        if (node->getIsExpanderNode() && node->getIsExpanded())
        {
            expandedNodeList.append(node);
        }
    }
    return expandedNodeList;
}
