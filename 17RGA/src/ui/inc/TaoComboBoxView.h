#ifndef TAOCOMBOBOXVIEW_H
#define TAOCOMBOBOXVIEW_H

#include <QListView>

class TaoComboBoxView : public QListView
{
    Q_OBJECT
public:
    explicit TaoComboBoxView(QWidget* parent = nullptr);
    ~TaoComboBoxView();
Q_SIGNALS:
    Q_SIGNAL void itemPressed(const QModelIndex& index);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // TAOCOMBOBOXVIEW_H
