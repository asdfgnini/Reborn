#ifndef TAOWINDOW_H
#define TAOWINDOW_H

#include <QMainWindow>

#include "../../include/Def.h"
#include "TaoAppBar.h"
#include "../../include/stdafx.h"

class TaoWindowPrivate;
class TAO_EXPORT TaoWindow : public QMainWindow
{
    Q_OBJECT
    Q_Q_CREATE(TaoWindow)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth)
    Q_PROPERTY_CREATE_Q_H(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_Q_H(bool, IsCentralStackedWidgetTransparent)
    Q_PROPERTY_CREATE_Q_H(TaoNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit TaoWindow(QWidget* parent = nullptr);
    ~TaoWindow();

    void moveToCenter();

    void setCustomWidget(TaoAppBarType::CustomArea customArea, QWidget* customWidget);
    QWidget* getCustomWidget() const;
    void setIsNavigationBarEnable(bool isEnable);
    bool getIsNavigationBarEnable() const;
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);
    TaoNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, TaoIconType::IconName awesome = TaoIconType::None) const;
    TaoNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, TaoIconType::IconName awesome = TaoIconType::None) const;
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, TaoIconType::IconName awesome = TaoIconType::None) const;
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, TaoIconType::IconName awesome = TaoIconType::None) const;
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None) const;
    TaoNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None) const;
    TaoNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None) const;
    TaoNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, TaoIconType::IconName awesome = TaoIconType::None) const;

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey);
    void setWindowButtonFlag(TaoAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(TaoAppBarType::ButtonFlags buttonFlags);
    TaoAppBarType::ButtonFlags getWindowButtonFlags() const;

    void closeWindow();
Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void navigationNodeClicked(TaoNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SIGNAL void customWidgetChanged();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
};

#endif // TAOWINDOW_H
