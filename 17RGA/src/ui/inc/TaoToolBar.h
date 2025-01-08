#ifndef TAOTOOLBAR_H
#define TAOTOOLBAR_H

#include <QToolBar>

#include "../../include/Def.h"
class TaoToolBarPrivate;
class TAO_EXPORT TaoToolBar : public QToolBar
{
    Q_OBJECT
    Q_Q_CREATE(TaoToolBar)
public:
    explicit TaoToolBar(QWidget* parent = nullptr);
    explicit TaoToolBar(const QString& title, QWidget* parent = nullptr);
    ~TaoToolBar();

    void setToolBarSpacing(int spacing);
    int getToolBarSpacing() const;

    QAction* addTaoIconAction(TaoIconType::IconName icon, const QString& text);
    QAction* addTaoIconAction(TaoIconType::IconName icon, const QString& text, const QKeySequence& shortcut);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // TAOTOOLBAR_H
