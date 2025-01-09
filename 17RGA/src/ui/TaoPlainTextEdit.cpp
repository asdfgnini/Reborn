#include "./inc/TaoPlainTextEdit.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>
#include <QPainter>
#include <QPropertyAnimation>

#include "./inc/TaoEventBus.h"
#include "./inc/TaoMenu.h"
#include "private/TaoPlainTextEditPrivate.h"
#include "./inc/TaoPlainTextEditStyle.h"
#include "./inc/TaoScrollBar.h"
#include "./inc/TaoTheme.h"
TaoPlainTextEdit::TaoPlainTextEdit(QWidget* parent)
    : QPlainTextEdit(parent), d_ptr(new TaoPlainTextEditPrivate())
{
    Q_D(TaoPlainTextEdit);
    d->q_ptr = this;
    setObjectName("TaoPlainTextEdit");
    setStyleSheet("#TaoPlainTextEdit{background-color:transparent;}");
    setHorizontalScrollBar(new TaoScrollBar(this));
    setVerticalScrollBar(new TaoScrollBar(this));
    setMouseTracking(true);
    // 事件总线
    d->_focusEvent = new TaoEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    d->_style = new TaoPlainTextEditStyle(style());
    setStyle(d->_style);
    d->onThemeChanged(tTheme->getThemeMode());
    connect(tTheme, &TaoTheme::themeModeChanged, d, &TaoPlainTextEditPrivate::onThemeChanged);
}

TaoPlainTextEdit::TaoPlainTextEdit(const QString& text, QWidget* parent)
    : TaoPlainTextEdit(parent)
{
    setPlainText(text);
}

TaoPlainTextEdit::~TaoPlainTextEdit()
{
}

void TaoPlainTextEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(TaoPlainTextEdit);
    if (event->reason() == Qt::MouseFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->_style, "pExpandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_style->getExpandMarkWidth());
        markAnimation->setEndValue(width() / 2 - 3);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPlainTextEdit::focusInEvent(event);
}

void TaoPlainTextEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(TaoPlainTextEdit);
    if (event->reason() != Qt::PopupFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->_style, "pExpandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_style->getExpandMarkWidth());
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPlainTextEdit::focusOutEvent(event);
}

void TaoPlainTextEdit::contextMenuEvent(QContextMenuEvent* event)
{
    TaoMenu* menu = new TaoMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!isReadOnly())
    {
        action = menu->addTaoIconAction(TaoIconType::ArrowRotateLeft, "撤销", QKeySequence::Undo);
        action->setEnabled(isUndoRedoEnabled() ? document()->isUndoAvailable() : false);
        connect(action, &QAction::triggered, this, &TaoPlainTextEdit::undo);

        action = menu->addTaoIconAction(TaoIconType::ArrowRotateRight, "恢复", QKeySequence::Redo);
        action->setEnabled(isUndoRedoEnabled() ? document()->isRedoAvailable() : false);
        connect(action, &QAction::triggered, this, &TaoPlainTextEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly())
    {
        action = menu->addTaoIconAction(TaoIconType::KnifeKitchen, "剪切", QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && !textCursor().selectedText().isEmpty());
        connect(action, &QAction::triggered, this, &TaoPlainTextEdit::cut);
    }

    action = menu->addTaoIconAction(TaoIconType::Copy, "复制", QKeySequence::Copy);
    action->setEnabled(!textCursor().selectedText().isEmpty());
    connect(action, &QAction::triggered, this, &TaoPlainTextEdit::copy);

    if (!isReadOnly())
    {
        action = menu->addTaoIconAction(TaoIconType::Paste, "粘贴", QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &TaoPlainTextEdit::paste);
    }
#endif
    if (!isReadOnly())
    {
        action = menu->addTaoIconAction(TaoIconType::DeleteLeft, "删除");
        action->setEnabled(!isReadOnly() && !toPlainText().isEmpty() && !textCursor().selectedText().isEmpty());
        connect(action, &QAction::triggered, this, [=](bool checked) {
            if (!textCursor().selectedText().isEmpty())
            {
                textCursor().deleteChar();
            }
        });
    }
    if (!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction("全选");
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!toPlainText().isEmpty() && !(textCursor().selectedText() == toPlainText()));
    connect(action, &QAction::triggered, this, &TaoPlainTextEdit::selectAll);
    menu->popup(event->globalPos());
    this->setFocus();
}
