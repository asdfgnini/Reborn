#include "./inc/TaoLineEdit.h"

#include <QClipboard>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "./inc/TaoEventBus.h"
#include "./inc/TaoLineEditStyle.h"
#include "./inc/TaoMenu.h"
#include "./inc/TaoTheme.h"
#include "private/TaoLineEditPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(TaoLineEdit, int, BorderRadius)

TaoLineEdit::TaoLineEdit(QWidget* parent)
    : QLineEdit(parent), d_ptr(new TaoLineEditPrivate())
{
    Q_D(TaoLineEdit);
    d->q_ptr = this;
    setObjectName("TaoLineEdit");
    d->_themeMode = tTheme->getThemeMode();
    d->_pBorderRadius = 6;
    d->_pExpandMarkWidth = 0;
    d->_pIsClearButtonEnable = true;
    setFocusPolicy(Qt::StrongFocus);
    // 事件总线
    d->_focusEvent = new TaoEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();
    setMouseTracking(true);
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, d->_textSpacing);
    setFont(textFont);
    setStyle(new TaoLineEditStyle(style()));
    setStyleSheet("#TaoLineEdit{padding-left: 10px;}");
    d->onThemeChanged(tTheme->getThemeMode());
    connect(tTheme, &TaoTheme::themeModeChanged, d, &TaoLineEditPrivate::onThemeChanged);
    setVisible(true);
}

TaoLineEdit::~TaoLineEdit()
{
}

void TaoLineEdit::setIsClearButtonEnable(bool isClearButtonEnable)
{
    Q_D(TaoLineEdit);
    d->_pIsClearButtonEnable = isClearButtonEnable;
    setClearButtonEnabled(isClearButtonEnable);
    Q_EMIT pIsClearButtonEnableChanged();
}

bool TaoLineEdit::getIsClearButtonEnable() const
{
    Q_D(const TaoLineEdit);
    return d->_pIsClearButtonEnable;
}

void TaoLineEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(TaoLineEdit);
    Q_EMIT focusIn(this->text());
    if (event->reason() == Qt::MouseFocusReason)
    {
        if (d->_pIsClearButtonEnable)
        {
            setClearButtonEnabled(true);
        }
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_pExpandMarkWidth);
        markAnimation->setEndValue(width() / 2 - d->_pBorderRadius / 2);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QLineEdit::focusInEvent(event);
}

void TaoLineEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(TaoLineEdit);
    Q_EMIT focusOut(this->text());
    if (event->reason() != Qt::PopupFocusReason)
    {
        if (d->_pIsClearButtonEnable)
        {
            setClearButtonEnabled(false);
        }
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
        connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_pExpandMarkWidth);
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        Q_EMIT wmFocusOut(text());
    }
    QLineEdit::focusOutEvent(event);
}

void TaoLineEdit::paintEvent(QPaintEvent* event)
{
    Q_D(TaoLineEdit);
    QLineEdit::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(TaoThemeColor(d->_themeMode, PrimaryNormal));
    painter.drawRoundedRect(QRectF(width() / 2 - d->_pExpandMarkWidth, height() - 2.5, d->_pExpandMarkWidth * 2, 2.5), 2, 2);
    painter.restore();
}

void TaoLineEdit::contextMenuEvent(QContextMenuEvent* event)
{
    TaoMenu* menu = new TaoMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!isReadOnly())
    {
        action = menu->addTaoIconAction(TaoIconType::TAOfile, tr("撤销"), QKeySequence::Undo);
        action->setEnabled(isUndoAvailable());
        connect(action, &QAction::triggered, this, &TaoLineEdit::undo);

        action = menu->addTaoIconAction(TaoIconType::TAOfile, tr("恢复"), QKeySequence::Redo);
        action->setEnabled(isRedoAvailable());
        connect(action, &QAction::triggered, this, &TaoLineEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly())
    {
        action = menu->addTaoIconAction(TaoIconType::TAOfile, tr("剪切"), QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && hasSelectedText() && echoMode() == QLineEdit::Normal);
        connect(action, &QAction::triggered, this, &TaoLineEdit::cut);
    }

    action = menu->addTaoIconAction(TaoIconType::TAOfile, tr("复制"), QKeySequence::Copy);
    action->setEnabled(hasSelectedText() && echoMode() == QLineEdit::Normal);
    connect(action, &QAction::triggered, this, &TaoLineEdit::copy);

    if (!isReadOnly())
    {
        action = menu->addTaoIconAction(TaoIconType::TAOfile, tr("粘贴"), QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        connect(action, &QAction::triggered, this, &TaoLineEdit::paste);
    }
#endif
    if (!isReadOnly())
    {
        action = menu->addTaoIconAction(TaoIconType::TAOfile, tr("删除"));
        action->setEnabled(!isReadOnly() && !text().isEmpty() && hasSelectedText());
        connect(action, &QAction::triggered, this, [=](bool checked) {
            if (hasSelectedText())
            {
                int startIndex = selectionStart();
                int endIndex = selectionEnd();
                setText(text().remove(startIndex, endIndex - startIndex));
            }
        });
    }
    if (!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction(tr("全选"));
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!text().isEmpty() && !(selectedText() == text()));
    connect(action, &QAction::triggered, this, &TaoLineEdit::selectAll);
    menu->popup(event->globalPos());
}
