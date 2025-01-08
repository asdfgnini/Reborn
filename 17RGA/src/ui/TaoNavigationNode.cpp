#include "./inc/TaoNavigationNode.h"

#include <QUuid>

TaoNavigationNode::TaoNavigationNode(QString nodeTitle, TaoNavigationNode* parent)
    : QObject(parent)
{
    _pDepth = 0;
    _pKeyPoints = 0;
    _nodeKey = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
    _nodeTitle = nodeTitle;
    _pIsRootNode = false;
    _pIsFooterNode = false;
    _pIsHasFooterPage = false;
    _pParentNode = parent;
    _pIsExpanderNode = false;
    _pIsVisible = false;
}

TaoNavigationNode::~TaoNavigationNode()
{
    qDeleteAll(_pChildrenNodes);
}

QString TaoNavigationNode::getNodeKey() const
{
    return _nodeKey;
}

QString TaoNavigationNode::getNodeTitle() const
{
    return _nodeTitle;
}

void TaoNavigationNode::setIsExpanded(bool isExpanded)
{
    _isExpanded = isExpanded;
    setChildVisible(isExpanded);
}

bool TaoNavigationNode::getIsExpanded() const
{
    return _isExpanded;
}

void TaoNavigationNode::setChildVisible(bool isVisible)
{
    if (isVisible)
    {
        for (auto node : _pChildrenNodes)
        {
            node->setIsVisible(isVisible);
            if (node->getIsExpanderNode() && !node->getIsExpanded())
            {
                continue;
            }
            node->setChildVisible(isVisible);
        }
    }
    else
    {
        for (auto node : _pChildrenNodes)
        {
            node->setChildVisible(isVisible);
            node->setIsVisible(isVisible);
        }
    }
}

bool TaoNavigationNode::getIsHasChild() const
{
    if (_pChildrenNodes.count() > 0)
    {
        return true;
    }
    return false;
}

bool TaoNavigationNode::getIsHasPageChild() const
{
    if (_pChildrenNodes.count() == 0)
    {
        return false;
    }
    for (auto childNode : _pChildrenNodes)
    {
        if (!childNode->getIsExpanderNode())
        {
            return true;
        }
        if (childNode->getIsHasPageChild())
        {
            return true;
        }
    }
    return false;
}

void TaoNavigationNode::appendChildNode(TaoNavigationNode* childNode)
{
    if (_pIsExpanderNode)
    {
        _pChildrenNodes.append(childNode);
    }
}

bool TaoNavigationNode::getIsChildHasKeyPoints() const
{
    for (auto childNnode : _pChildrenNodes)
    {
        if (childNnode->getKeyPoints())
        {
            return true;
        }
        if (childNnode->getIsChildHasKeyPoints())
        {
            return true;
        }
    }
    return false;
}

TaoNavigationNode* TaoNavigationNode::getOriginalNode()
{
    if (this->getParentNode()->getIsRootNode())
    {
        return this;
    }
    else
    {
        TaoNavigationNode* originalNode = this->getParentNode();
        while (!originalNode->getIsRootNode() && !originalNode->getParentNode()->getIsRootNode())
        {
            originalNode = originalNode->getParentNode();
        }
        return originalNode;
    }
}

bool TaoNavigationNode::getIsChildNode(TaoNavigationNode* node)
{
    if (_pChildrenNodes.count() > 0)
    {
        if (_pChildrenNodes.contains(node))
        {
            return true;
        }
        for (auto childNnode : _pChildrenNodes)
        {
            if (childNnode->getIsChildNode(node))
            {
                return true;
            }
        }
    }
    return false;
}

int TaoNavigationNode::getRow() const
{
    if (_pParentNode)
    {
        return _pParentNode->getChildrenNodes().indexOf(const_cast<TaoNavigationNode*>(this));
    }
    return 0;
}
