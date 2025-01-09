#include "./inc/TaoSpinBox.h"

#include <QContextMenuEvent>
#include <QLineEdit>
#include <QPainter>

#include "./inc/TaoSpinBoxStyle.h"
#include "./inc/TaoMenu.h"
#include "private/TaoSpinBoxPrivate.h"
#include "./inc/TaoTheme.h"

TaoSpinBox::TaoSpinBox(QWidget* parent)
    : QSpinBox(parent), d_ptr(new TaoSpinBoxPrivate())
{
    Q_D(TaoSpinBox);
    d->q_ptr = this;
    setFixedSize(120, 30);
    setStyle(new TaoSpinBoxStyle(style()));
    lineEdit()->setAlignment(Qt::AlignCenter);
    lineEdit()->setStyleSheet("background-color:transparent");
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::transparent);
        palette.setColor(QPalette::Text, TaoThemeColor(themeMode, BasicText));
        lineEdit()->setPalette(palette);
    });
}

TaoSpinBox::~TaoSpinBox()
{
}

void TaoSpinBox::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(TaoSpinBox);
    TaoMenu* menu = d->_createStandardContextMenu();
    if (!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addTaoIconAction(TaoIconType::Plus, tr("增加"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addTaoIconAction(TaoIconType::Minus, tr("减少"));
    down->setEnabled(se & StepDownEnabled);
    menu->addSeparator();

    const QAbstractSpinBox* that = this;
    const QPoint pos = (event->reason() == QContextMenuEvent::Mouse)
                           ? event->globalPos()
                           : mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(width() / 2, height() / 2);
    const QAction* action = menu->exec(pos);
    delete menu;
    if (that && action)
    {
        if (action == up)
        {
            stepBy(1);
        }
        else if (action == down)
        {
            stepBy(-1);
        }
    }
    event->accept();
}
