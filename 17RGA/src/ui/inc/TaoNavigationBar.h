#ifndef TAONAVIGATIONBAR_H
#define TAONAVIGATIONBAR_H

#include <QWidget>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoNavigationBarPrivate;
class TAO_EXPORT TaoNavigationBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(TaoNavigationBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
public:
    explicit TaoNavigationBar(QWidget* parent = nullptr);
    ~TaoNavigationBar();
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);

    TaoNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, TaoIconType::IconName awesome = TaoIconType::None);
    TaoNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, TaoIconType::IconName awesome = TaoIconType::None);
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, TaoIconType::IconName awesome = TaoIconType::None);
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, TaoIconType::IconName awesome = TaoIconType::None);
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None);
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None);
    TaoNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None);
    TaoNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None);

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey, bool isLogClicked = true);
    void setDisplayMode(TaoNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);

Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void navigationNodeClicked(TaoNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SIGNAL void navigationNodeAdded(TaoNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAONAVIGATIONBAR_H
