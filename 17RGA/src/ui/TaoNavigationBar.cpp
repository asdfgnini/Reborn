#include "./inc/TaoNavigationBar.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScroller>
#include <QVBoxLayout>

#include "./inc/TaoBaseListView.h"
#include "./inc/TaoFooterDelegate.h"
#include "./inc/TaoFooterModel.h"
#include "./inc/TaoIconButton.h"
#include "./inc/TaoInteractiveCard.h"
#include "./inc/TaoMenu.h"
#include "./inc/TaoNavigationModel.h"
#include "./inc/TaoNavigationNode.h"
#include "./inc/TaoNavigationView.h"
#include "./inc/TaoSuggestBox.h"
#include "./inc/TaoTheme.h"
#include "./inc/TaoToolButton.h"
#include "private/TaoNavigationBarPrivate.h"
#include "private/TaoSuggestBoxPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(TaoNavigationBar, bool, IsTransparent)
TaoNavigationBar::TaoNavigationBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new TaoNavigationBarPrivate())
{
    Q_D(TaoNavigationBar);
    d->q_ptr = this;
    setFixedWidth(300);
    d->_pIsTransparent = true;

    //用户卡片
    d->_userCard = new TaoInteractiveCard(this);
    d->_userCard->setCardPixmap(QPixmap(":/ui/res/Image/ReBorn.png"));
    d->_userCard->setTitle("ReBorn Tool");
    d->_userCard->setSubTitle("https://www.cdreborn.com/");
    connect(d->_userCard, &TaoInteractiveCard::clicked, this, &TaoNavigationBar::userInfoCardClicked);
    
    d->_userButton = new TaoIconButton(QPixmap(":/ui/res/Image/ReBorn.png"), this);
    d->_userButton->setFixedSize(36, 36);
    d->_userButton->setVisible(false);
    d->_userButton->setBorderRadius(8);
    d->_userButtonLayout = new QVBoxLayout();
    d->_userButtonLayout->setAlignment(Qt::AlignLeft);
    d->_userButtonLayout->setContentsMargins(0, 0, 0, 6);
    d->_userButtonLayout->setSpacing(0);
    d->_userButtonLayout->addWidget(d->_userButton);
    connect(d->_userButton, &TaoIconButton::clicked, this, &TaoNavigationBar::userInfoCardClicked);
    
    QHBoxLayout* userCardLayout = new QHBoxLayout();
    userCardLayout->setContentsMargins(0, 0, 0, 0);
    userCardLayout->addSpacing(3);
    userCardLayout->addLayout(d->_userButtonLayout);
    userCardLayout->addWidget(d->_userCard);

    // 搜索栏和按钮组
    d->_navigationButton = new TaoToolButton(this);
    d->_navigationButton->setFixedSize(40, 38);
    d->_navigationButton->setTaoIcon(TaoIconType::Bars);
    d->_navigationButton->setBorderRadius(8);
    connect(d->_navigationButton, &TaoToolButton::clicked, d, &TaoNavigationBarPrivate::onNavigationButtonClicked);

    d->_searchButton = new TaoToolButton(this);
    d->_searchButton->setFixedSize(40, 38);
    d->_searchButton->setTaoIcon(TaoIconType::MagnifyingGlass);
    d->_searchButton->setBorderRadius(8);
    connect(d->_searchButton, &TaoToolButton::clicked, d, &TaoNavigationBarPrivate::onNavigationButtonClicked);
    d->_searchButton->setVisible(false);

    d->_navigationSuggestBox = new TaoSuggestBox(this);
    d->_navigationSuggestBox->setMinimumWidth(0);
    d->_navigationButtonLayout = new QVBoxLayout();
    d->_navigationButtonLayout->setContentsMargins(0, 0, 0, 0);
    d->_navigationButtonLayout->setSpacing(0);
    d->_navigationButtonLayout->addWidget(d->_navigationButton);
    d->_navigationButtonLayout->addWidget(d->_searchButton);

    d->_navigationSuggestLayout = new QHBoxLayout();
    d->_navigationSuggestLayout->setContentsMargins(0, 0, 10, 0);
    d->_navigationSuggestLayout->setSpacing(6);
    d->_navigationSuggestLayout->addLayout(d->_navigationButtonLayout);
    d->_navigationSuggestLayout->addWidget(d->_navigationSuggestBox);

    // 搜索跳转
    connect(d->_navigationSuggestBox, &TaoSuggestBox::suggestionClicked, this, [=](QString suggestText, QVariantMap suggestData) {
        TaoNavigationNode* node = nullptr;
        if (suggestData.value("TaoNodeType").toString() == "Stacked")
        {
            node = d->_navigationModel->getNavigationNode(suggestData.value("TaoPageKey").toString());
            if (node)
            {
                d->onTreeViewClicked(node->getModelIndex());
            }
        }
        else
        {
            node = d->_footerModel->getNavigationNode(suggestData.value("TaoPageKey").toString());
            if (node)
            {
                d->onFooterViewClicked(node->getModelIndex());
            }
        }
    });

    // 导航模型
    d->_navigationModel = new TaoNavigationModel(this);
    d->_navigationView = new TaoNavigationView(this);
    d->_navigationView->setModel(d->_navigationModel);
    connect(d->_navigationView, &TaoNavigationView::navigationClicked, this, [=](const QModelIndex& index) { d->onTreeViewClicked(index); });
    connect(d->_navigationView, &TaoNavigationView::navigationOpenNewWindow, d, &TaoNavigationBarPrivate::onNavigationOpenNewWindow);

    // 页脚
    d->_footerView = new TaoBaseListView(this);
    d->_footerView->setFixedHeight(0);
    d->_footerModel = new TaoFooterModel(this);
    d->_footerView->setModel(d->_footerModel);
    d->_footerDelegate = new TaoFooterDelegate(this);
    d->_footerDelegate->setTaoListView(d->_footerView);
    d->_footerView->setItemDelegate(d->_footerDelegate);
    connect(d->_footerView, &TaoBaseListView::mousePress, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(index);
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &TaoBaseListView::mouseDoubleClick, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(index);
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &TaoBaseListView::mouseRelease, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(QModelIndex());
        d->_footerView->viewport()->update();
    });
    connect(d->_footerView, &TaoBaseListView::clicked, this, [=](const QModelIndex& index) { d->onFooterViewClicked(index); });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 10, 5, 0);
    mainLayout->addLayout(userCardLayout);
    mainLayout->addSpacing(4);
    mainLayout->addLayout(d->_navigationSuggestLayout);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(d->_navigationView);
    mainLayout->addWidget(d->_footerView);

    //主题设置
    d->_themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

TaoNavigationBar::~TaoNavigationBar()
{
}

void TaoNavigationBar::setUserInfoCardVisible(bool isVisible)
{

    Q_D(TaoNavigationBar);
    d->_isShowUserCard = isVisible;
    d->_userCard->setVisible(isVisible);
    if (!isVisible)
    {
        d->_userButton->setVisible(false);
    }
}

void TaoNavigationBar::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(TaoNavigationBar);
    d->_userCard->setCardPixmap(pix);
    d->_userButton->setPixmap(pix);
}

void TaoNavigationBar::setUserInfoCardTitle(QString title)
{
    Q_D(TaoNavigationBar);
    d->_userCard->setTitle(title);
}

void TaoNavigationBar::setUserInfoCardSubTitle(QString subTitle)
{
    Q_D(TaoNavigationBar);
    d->_userCard->setSubTitle(subTitle);
}

TaoNavigationType::NodeOperateReturnType TaoNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, TaoIconType::IconName awesome)
{
    Q_D(TaoNavigationBar);
    TaoNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addExpanderNode(expanderTitle, expanderKey, awesome);
    if (returnType == TaoNavigationType::Success)
    {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, TaoIconType::IconName awesome)
{
    Q_D(TaoNavigationBar);
    TaoNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
    if (returnType == TaoNavigationType::Success)
    {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationBar::addPageNode(QString pageTitle, QWidget* page, TaoIconType::IconName awesome)
{
    Q_D(TaoNavigationBar);
    if (!page)
    {
        return TaoNavigationType::PageInvalid;
    }
    QString pageKey;
    TaoNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, awesome);
    if (returnType == TaoNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, TaoIconType::IconName awesome)
{
    Q_D(TaoNavigationBar);
    if (!page)
    {
        return TaoNavigationType::PageInvalid;
    }
    if (targetExpanderKey.isEmpty())
    {
        return TaoNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    TaoNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awesome);
    if (returnType == TaoNavigationType::NodeOperateReturnType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        TaoNavigationNode* node = d->_navigationModel->getNavigationNode(pageKey);
        TaoNavigationNode* originalNode = node->getOriginalNode();
        if (d->_compactMenuMap.contains(originalNode))
        {
            TaoMenu* menu = d->_compactMenuMap.value(originalNode);
            QAction* action = menu->addTaoIconAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d->onTreeViewClicked(node->getModelIndex());
            });
        }
        else
        {
            TaoMenu* menu = new TaoMenu(const_cast<TaoNavigationBar*>(this));
            QAction* action = menu->addTaoIconAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
            d_ptr->_compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationBar::addPageNode(QString pageTitle, QWidget* page, int keyPoints, TaoIconType::IconName awesome)
{
    Q_D(TaoNavigationBar);
    if (!page)
    {
        return TaoNavigationType::PageInvalid;
    }
    QString pageKey;
    TaoNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, keyPoints, awesome);
    if (returnType == TaoNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, TaoIconType::IconName awesome)
{
    Q_D(TaoNavigationBar);
    if (!page)
    {
        return TaoNavigationType::PageInvalid;
    }
    if (targetExpanderKey.isEmpty())
    {
        return TaoNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    TaoNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, awesome);
    if (returnType == TaoNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        TaoNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
        TaoNavigationNode* originalNode = node->getOriginalNode();
        if (d_ptr->_compactMenuMap.contains(originalNode))
        {
            TaoMenu* menu = d_ptr->_compactMenuMap.value(originalNode);
            QAction* action = menu->addTaoIconAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
        }
        else
        {
            TaoMenu* menu = new TaoMenu(const_cast<TaoNavigationBar*>(this));
            QAction* action = menu->addTaoIconAction(node->getAwesome(), node->getNodeTitle());
            connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
            d_ptr->_compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

TaoNavigationType::NodeOperateReturnType TaoNavigationBar::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, TaoIconType::IconName awesome)
{
    return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

TaoNavigationType::NodeOperateReturnType TaoNavigationBar::addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, TaoIconType::IconName awesome)
{
    TaoNavigationType::NodeOperateReturnType returnType = d_ptr->_footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, awesome);
    if (returnType == TaoNavigationType::Success)
    {
        d_ptr->_addFooterPage(page, footerKey);
    }
    return returnType;
}

void TaoNavigationBar::setNodeKeyPoints(QString nodeKey, int keyPoints)
{
    Q_D(TaoNavigationBar);
    TaoNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node || node->getIsExpanderNode() || keyPoints < 0)
    {
        return;
    }
    node->setKeyPoints(keyPoints);
    if (node->getIsFooterNode())
    {
        d->_footerView->viewport()->update();
    }
    else
    {
        d->_navigationView->viewport()->update();
    }
}

int TaoNavigationBar::getNodeKeyPoints(QString nodeKey) const
{
    Q_D(const TaoNavigationBar);
    TaoNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node || node->getIsExpanderNode())
    {
        return -1;
    }
    return node->getKeyPoints();
}

void TaoNavigationBar::navigation(QString pageKey, bool isLogClicked)
{
    Q_D(TaoNavigationBar);
    TaoNavigationNode* node = d->_navigationModel->getNavigationNode(pageKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(pageKey);
    }
    if (node)
    {
        if (node->getIsFooterNode())
        {
            d->onFooterViewClicked(node->getModelIndex(), isLogClicked);
        }
        else
        {
            if (!node->getIsExpanderNode())
            {
                d->onTreeViewClicked(node->getModelIndex(), isLogClicked);
            }
        }
    }
}

void TaoNavigationBar::setDisplayMode(TaoNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    Q_D(TaoNavigationBar);
    if (d->_currentDisplayMode == displayMode || displayMode == TaoNavigationType::Auto)
    {
        return;
    }
    d->_doComponentAnimation(displayMode, isAnimation);
    d->_raiseNavigationBar();
}

void TaoNavigationBar::paintEvent(QPaintEvent* event)
{
    Q_D(TaoNavigationBar);
    if (!d->_pIsTransparent)
    {
        QPainter painter(this);
        painter.save();
        painter.setPen(TaoThemeColor(d->_themeMode, PopupBorder));
        painter.setBrush(TaoThemeColor(d->_themeMode, PopupBase));
        QRect baseRect = this->rect();
        baseRect.adjust(-1, 0, -1, 0);
        QPainterPath path;
        path.moveTo(baseRect.topLeft());
        path.lineTo(QPoint(baseRect.right() - 8, baseRect.y()));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.y(), 16, 16), 90, -90);
        path.lineTo(QPoint(baseRect.right(), baseRect.bottom() - 8));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.bottom() - 16, 16, 16), 0, -90);
        path.lineTo(baseRect.bottomLeft());
        path.closeSubpath();
        painter.drawPath(path);
        painter.restore();
    }
    QWidget::paintEvent(event);
}
