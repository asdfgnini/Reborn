#ifndef TAOBREADCRUMBBARMODEL_H
#define TAOBREADCRUMBBARMODEL_H

#include <QAbstractListModel>

class TaoBreadcrumbBarModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TaoBreadcrumbBarModel(QObject* parent = nullptr);
    ~TaoBreadcrumbBarModel();

    void appendBreadcrumb(QString breadcrumb);
    void removeBreadcrumb(QString breadcrumb);
    void removeBreadcrumb(int index);

    void setBreadcrumbList(QStringList breadcrumbList);

    int getBreadcrumbListCount();
    QStringList getBreadcrumbList();

protected:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    QStringList _breadcrumbList;
};

#endif // TAOBREADCRUMBBARMODEL_H
