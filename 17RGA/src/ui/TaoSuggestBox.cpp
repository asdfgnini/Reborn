#include "./inc/TaoSuggestBox.h"

#include <QAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QMap>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>

#include "./inc/TaoBaseListView.h"
#include "./inc/TaoIcon.h"
#include "./inc/TaoLineEdit.h"
#include "./inc/TaoScrollBar.h"
#include "./inc/TaoSuggestBoxSearchViewContainer.h"
#include "./inc/TaoSuggestDelegate.h"
#include "./inc/TaoSuggestModel.h"
#include "./inc/TaoTheme.h"
#include "private/TaoSuggestBoxPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(TaoSuggestBox, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(TaoSuggestBox, Qt::CaseSensitivity, CaseSensitivity)
TaoSuggestBox::TaoSuggestBox(QWidget* parent)
    : QWidget{parent}, d_ptr(new TaoSuggestBoxPrivate())
{
    Q_D(TaoSuggestBox);
    setFixedSize(280, 35);
    d->q_ptr = this;
    d->_pBorderRadius = 6;
    d->_pCaseSensitivity = Qt::CaseInsensitive;
    d->_searchEdit = new TaoLineEdit(this);
    d->_searchEdit->setFixedHeight(35);
    d->_searchEdit->setPlaceholderText("查找功能");
    d->_searchEdit->setClearButtonEnabled(true);
    d->_lightSearchAction = new QAction(TaoIcon::getInstance()->getTaoIcon(TaoIconType::MagnifyingGlass), "Search", this);
    d->_darkSearchAction = new QAction(TaoIcon::getInstance()->getTaoIcon(TaoIconType::MagnifyingGlass, QColor(0xFF, 0xFF, 0xFF)), "Search", this);

    d->_themeMode = tTheme->getThemeMode();
    connect(tTheme, &TaoTheme::themeModeChanged, d, &TaoSuggestBoxPrivate::onThemeModeChanged);
    if (d->_themeMode == TaoThemeType::Light)
    {
        d->_searchEdit->addAction(d->_lightSearchAction, QLineEdit::TrailingPosition);
    }
    else
    {
        d->_searchEdit->addAction(d->_darkSearchAction, QLineEdit::TrailingPosition);
    }

    connect(d->_lightSearchAction, &QAction::triggered, this, [=](bool checked) {
        //qDebug() << "Search";
    });

    connect(d->_darkSearchAction, &QAction::triggered, this, [=](bool checked) {
        //qDebug() << "Search";
    });
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(d->_searchEdit);
    d->_searchViewBaseWidget = new TaoSuggestBoxSearchViewContainer(window());
    d->_shadowLayout = new QVBoxLayout(d->_searchViewBaseWidget);
    d->_shadowLayout->setContentsMargins(0, 0, 0, 0);
    d->_searchView = new TaoBaseListView(d->_searchViewBaseWidget);
    TaoScrollBar* floatVScrollBar = new TaoScrollBar(d->_searchView->verticalScrollBar(), d->_searchView);
    floatVScrollBar->setIsAnimation(true);
    d->_searchViewBaseWidget->resize(292, 300);
    d->_shadowLayout->addWidget(d->_searchView);
    d->_searchModel = new TaoSuggestModel(this);
    d->_searchDelegate = new TaoSuggestDelegate(this);
    d->_searchView->setModel(d->_searchModel);
    d->_searchView->setItemDelegate(d->_searchDelegate);
    d->_searchViewBaseWidget->hide();
    connect(d->_searchEdit, &TaoLineEdit::textEdited, d, &TaoSuggestBoxPrivate::onSearchEditTextEdit);
    connect(d->_searchEdit, &TaoLineEdit::focusIn, d, &TaoSuggestBoxPrivate::onSearchEditTextEdit);
    connect(d->_searchView, &TaoBaseListView::clicked, d, &TaoSuggestBoxPrivate::onSearchViewClicked);

    // 焦点事件
    connect(d->_searchEdit, &TaoLineEdit::wmFocusOut, this, [d]() {
        d->_startCloseAnimation();
    });
}

TaoSuggestBox::~TaoSuggestBox()
{
}

void TaoSuggestBox::setPlaceholderText(const QString& placeholderText)
{
    Q_D(TaoSuggestBox);
    d->_searchEdit->setPlaceholderText(placeholderText);
}

void TaoSuggestBox::addSuggestion(const QString& suggestText, const QVariantMap& suggestData)
{
    Q_D(TaoSuggestBox);
    TaoSuggestion* suggest = new TaoSuggestion(this);
    suggest->setSuggestText(suggestText);
    suggest->setSuggestData(suggestData);
    d->_suggestionVector.append(suggest);
}

void TaoSuggestBox::addSuggestion(TaoIconType::IconName icon, const QString& suggestText, const QVariantMap& suggestData)
{
    Q_D(TaoSuggestBox);
    TaoSuggestion* suggest = new TaoSuggestion(this);
    suggest->setTaoIcon(icon);
    suggest->setSuggestText(suggestText);
    suggest->setSuggestData(suggestData);
    d->_suggestionVector.append(suggest);
}

void TaoSuggestBox::removeSuggestion(const QString& suggestText)
{
    Q_D(TaoSuggestBox);
    foreach (auto suggest, d->_suggestionVector)
    {
        if (suggest->getSuggestText() == suggestText)
        {
            d->_suggestionVector.removeOne(suggest);
            suggest->deleteLater();
        }
    }
}

void TaoSuggestBox::removeSuggestion(int index)
{
    Q_D(TaoSuggestBox);
    if (index >= d->_suggestionVector.count())
    {
        return;
    }
    TaoSuggestion* suggest = d->_suggestionVector[index];
    d->_suggestionVector.removeOne(suggest);
    suggest->deleteLater();
}
