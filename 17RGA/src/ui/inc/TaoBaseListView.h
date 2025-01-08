#ifndef TAOBASELISTVIEW_H
#define TAOBASELISTVIEW_H

#include <QListView>
#include <QModelIndex>

class TaoScrollBar;
class TaoBaseListView : public QListView
{
    Q_OBJECT
public:
    explicit TaoBaseListView(QWidget* parent = nullptr);
    ~TaoBaseListView();
Q_SIGNALS:
    Q_SIGNAL void mousePress(const QModelIndex& index);
    Q_SIGNAL void mouseRelease(const QModelIndex& index);
    Q_SIGNAL void mouseDoubleClick(const QModelIndex& index);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // TAOBASELISTVIEW_H
