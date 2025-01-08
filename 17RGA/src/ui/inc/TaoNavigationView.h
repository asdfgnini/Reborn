#ifndef TAONAVIGATIONVIEW_H
#define TAONAVIGATIONVIEW_H

#include <QObject>
#include <QTreeView>

class TaoScrollBar;
class TaoNavigationStyle;
class TaoNavigationView : public QTreeView
{
    Q_OBJECT
public:
    explicit TaoNavigationView(QWidget* parent = nullptr);
    ~TaoNavigationView();
    void navigationNodeStateChange(QVariantMap data);

    Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);
Q_SIGNALS:
    Q_SIGNAL void navigationClicked(const QModelIndex& index);
    Q_SIGNAL void navigationOpenNewWindow(QString nodeKey);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    TaoNavigationStyle* _navigationStyle{nullptr};
};

#endif // TAONAVIGATIONVIEW_H
