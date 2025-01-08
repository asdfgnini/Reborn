#include "./inc/TaoSuggestModel.h"

TaoSuggestModel::TaoSuggestModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

TaoSuggestModel::~TaoSuggestModel()
{
}

int TaoSuggestModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return _suggestionVector.count();
}

QVariant TaoSuggestModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void TaoSuggestModel::setSearchSuggestion(QVector<TaoSuggestion*> suggestionVector)
{
    if (suggestionVector.count() == 0)
    {
        return;
    }
    beginResetModel();
    _suggestionVector = suggestionVector;
    endResetModel();
}

void TaoSuggestModel::clearSearchNode()
{
    this->_suggestionVector.clear();
}

TaoSuggestion* TaoSuggestModel::getSearchSuggestion(int row)
{
    if (row >= _suggestionVector.count())
    {
        return nullptr;
    }
    return _suggestionVector[row];
}
