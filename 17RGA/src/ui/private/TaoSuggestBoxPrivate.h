#ifndef TAOSUGGESTBOXPRIVATE_H
#define TAOSUGGESTBOXPRIVATE_H

#include <QAction>
#include <QObject>
#include <QSize>
#include <QVariantMap>
#include <QVector>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoSuggestion : public QObject
{
    Q_OBJECT
    Q_PROPERTY_CREATE(TaoIconType::IconName, TaoIcon)
    Q_PROPERTY_CREATE(QString, SuggestText)
    Q_PROPERTY_CREATE(QVariantMap, SuggestData)
public:
    explicit TaoSuggestion(QObject* parent = nullptr);
    ~TaoSuggestion();
};

class QVBoxLayout;
class TaoLineEdit;
class TaoNavigationNode;
class TaoSuggestModel;
class TaoBaseListView;
class TaoSuggestDelegate;
class TaoSuggestBox;
class TaoSuggestBoxSearchViewContainer;
class TaoSuggestBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoSuggestBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(Qt::CaseSensitivity, CaseSensitivity)
public:
    explicit TaoSuggestBoxPrivate(QObject* parent = nullptr);
    ~TaoSuggestBoxPrivate();
    Q_SLOT void onThemeModeChanged(TaoThemeType::ThemeMode themeMode);
    Q_SLOT void onSearchEditTextEdit(const QString& searchText);
    Q_SLOT void onSearchViewClicked(const QModelIndex& index);

private:
    TaoThemeType::ThemeMode _themeMode;
    QAction* _lightSearchAction{nullptr};
    QAction* _darkSearchAction{nullptr};
    QVector<TaoSuggestion*> _suggestionVector;
    TaoSuggestBoxSearchViewContainer* _searchViewBaseWidget{nullptr};
    TaoLineEdit* _searchEdit{nullptr};
    TaoSuggestModel* _searchModel{nullptr};
    TaoBaseListView* _searchView{nullptr};
    TaoSuggestDelegate* _searchDelegate{nullptr};
    QVBoxLayout* _shadowLayout{nullptr};
    QSize _lastSize;
    bool _isExpandAnimationFinished{true};
    bool _isCloseAnimationFinished{true};
    void _startSizeAnimation(QSize oldSize, QSize newSize);
    void _startExpandAnimation();
    void _startCloseAnimation();
};

#endif // TAOSUGGESTBOXPRIVATE_H
