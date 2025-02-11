#include "./inc/TaoBreadcrumbBarModel.h"

TaoBreadcrumbBarModel::TaoBreadcrumbBarModel(QObject* parent)
    : QAbstractListModel{parent}
{
}

TaoBreadcrumbBarModel::~TaoBreadcrumbBarModel()
{
}

int TaoBreadcrumbBarModel::rowCount(const QModelIndex& parent) const
{
    return _breadcrumbList.count() * 2 - 1;
}

QVariant TaoBreadcrumbBarModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.row() % 2 == 0)
        {
            return _breadcrumbList[index.row() / 2];
        }
        else
        {
            return ">";
        }
    }
    else if (role == Qt::UserRole)
    {
        if (index.row() == _breadcrumbList.count() * 2 - 2 || index.row() == _breadcrumbList.count() * 2 - 3)
        {
            return QString("LastBreadcrumb");
        }
    }
    return QVariant();
}

void TaoBreadcrumbBarModel::appendBreadcrumb(QString breadcrumb)
{
    if (!breadcrumb.isEmpty())
    {
        if (_breadcrumbList.count() && _breadcrumbList.last() == breadcrumb)
        {
            return;
        }
        if (_breadcrumbList.count() >= 2 && _breadcrumbList[_breadcrumbList.count() - 2] == breadcrumb)
        {
            beginResetModel();
            _breadcrumbList.removeLast();
            endResetModel();
            return;
        }
        beginResetModel();
        _breadcrumbList.append(breadcrumb);
        endResetModel();
    }
}

void TaoBreadcrumbBarModel::removeBreadcrumb(QString breadcrumb)
{
    if (_breadcrumbList.contains(breadcrumb))
    {
        beginResetModel();
        _breadcrumbList.removeAt(_breadcrumbList.lastIndexOf(breadcrumb));
        endResetModel();
    }
}

void TaoBreadcrumbBarModel::removeBreadcrumb(int index)
{
    if (index >= _breadcrumbList.count())
    {
        return;
    }
    beginResetModel();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    _breadcrumbList.remove(index, _breadcrumbList.count() - index);
#else
    for (int i = _breadcrumbList.count() - 1; i >= 0; i--)
    {
        if (i < index)
        {
            break;
        }
        _breadcrumbList.removeAt(i);
    }
#endif
    endResetModel();
}

void TaoBreadcrumbBarModel::setBreadcrumbList(QStringList breadcrumbList)
{
    beginResetModel();
    this->_breadcrumbList = breadcrumbList;
    endResetModel();
}

int TaoBreadcrumbBarModel::getBreadcrumbListCount()
{
    return _breadcrumbList.count();
}

QStringList TaoBreadcrumbBarModel::getBreadcrumbList()
{
    return _breadcrumbList;
}
