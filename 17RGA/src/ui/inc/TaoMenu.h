#ifndef TAOMENU_H
#define TAOMENU_H

#include <QMenu>
#include <QWidget>


#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoMenuPrivate;
class TAO_EXPORT TaoMenu : public QMenu
{
    Q_OBJECT
    Q_Q_CREATE(TaoMenu)

public:
    explicit TaoMenu(QWidget* parent = nullptr);
    explicit TaoMenu(const QString& title, QWidget* parent = nullptr);
    ~TaoMenu();
    void setMenuItemHeight(int menuItemHeight);
    int getMenuItemHeight() const;

    QAction* addMenu(QMenu* menu);
    TaoMenu* addMenu(const QString& title);
    TaoMenu* addMenu(const QIcon& icon, const QString& title);
    TaoMenu* addMenu(TaoIconType::IconName icon, const QString& title);

    QAction* addTaoIconAction(TaoIconType::IconName icon, const QString& text);
    QAction* addTaoIconAction(TaoIconType::IconName icon, const QString& text, const QKeySequence& shortcut);

    bool isHasChildMenu() const;
    bool isHasIcon() const;
Q_SIGNALS:
    Q_SIGNAL void menuShow();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOMENU_H
