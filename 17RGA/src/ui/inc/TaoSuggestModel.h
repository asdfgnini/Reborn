#ifndef TAOSUGGESTMODEL_H
#define TAOSUGGESTMODEL_H

#include <QAbstractListModel>

#include "../../include/Def.h"
class TaoSuggestion;
class TaoSuggestModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TaoSuggestModel(QObject* parent = nullptr);
    ~TaoSuggestModel();
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    void setSearchSuggestion(QVector<TaoSuggestion*> suggestionVector);
    void clearSearchNode();
    TaoSuggestion* getSearchSuggestion(int row);

private:
    QVector<TaoSuggestion*> _suggestionVector; //符合搜索的节点
};

#endif // TAOSUGGESTMODEL_H
