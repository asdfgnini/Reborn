#ifndef TAONAVIGATIONNODE_H
#define TAONAVIGATIONNODE_H

#include <QModelIndex>
#include <QObject>

#include "../../include/Def.h"

class TaoNavigationNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE(QList<TaoNavigationNode*>, ChildrenNodes)
    Q_PRIVATE_CREATE(TaoNavigationNode*, ParentNode)
    Q_PROPERTY_CREATE(TaoIconType::IconName, Awesome)
    Q_PROPERTY_CREATE(QModelIndex, ModelIndex)
    Q_PROPERTY_CREATE(int, KeyPoints)
    Q_PROPERTY_CREATE(int, Depth)
    Q_PROPERTY_CREATE(bool, IsRootNode)
    Q_PROPERTY_CREATE(bool, IsFooterNode)
    Q_PROPERTY_CREATE(bool, IsHasFooterPage)
    Q_PROPERTY_CREATE(bool, IsExpanderNode)
    Q_PROPERTY_CREATE(bool, IsVisible)
public:
    explicit TaoNavigationNode(QString nodeTitle, TaoNavigationNode* parent = nullptr);
    ~TaoNavigationNode();

    QString getNodeKey() const;
    QString getNodeTitle() const;

    void setIsExpanded(bool isExpanded);
    bool getIsExpanded() const;

    void setChildVisible(bool isVisible);
    bool getIsHasChild() const;
    bool getIsHasPageChild() const;

    void appendChildNode(TaoNavigationNode* childNode);

    bool getIsChildHasKeyPoints() const;

    TaoNavigationNode* getOriginalNode();
    bool getIsChildNode(TaoNavigationNode* node);

    int getRow() const;

private:
    QString _nodeKey = "";
    QString _nodeTitle = "";
    bool _isExpanded{false};
};

#endif // TAONAVIGATIONNODE_H
