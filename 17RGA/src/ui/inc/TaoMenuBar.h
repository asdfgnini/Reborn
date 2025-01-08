#ifndef TAOMENUBAR_H
#define TAOMENUBAR_H

#include <QMenuBar>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoMenu;
class TAO_EXPORT TaoMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit TaoMenuBar(QWidget* parent = nullptr);
    ~TaoMenuBar();

    QAction* addMenu(QMenu* menu);
    TaoMenu* addMenu(const QString& title);
    TaoMenu* addMenu(const QIcon& icon, const QString& title);
    TaoMenu* addMenu(TaoIconType::IconName, const QString& title);

    QAction* addTaoIconAction(TaoIconType::IconName icon, const QString& text);
    QAction* addTaoIconAction(TaoIconType::IconName icon, const QString& text, const QKeySequence& shortcut);
};

#endif // TAOMENUBAR_H
