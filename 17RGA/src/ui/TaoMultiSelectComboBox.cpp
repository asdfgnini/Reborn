#include "./inc/TaoMultiSelectComboBox.h"

#include <QApplication>
#include <QDebug>
#include <QLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include "./inc/TaoComboBoxView.h"
#include "./inc/TaoApplication.h"
#include "./inc/TaoComboBoxStyle.h"
#include "./inc/TaoScrollBar.h"
#include "./inc/TaoTheme.h"
#include "private/TaoMultiSelectComboBoxPrivate.h"

Q_PROPERTY_CREATE_Q_CPP(TaoMultiSelectComboBox, int, BorderRadius)
TaoMultiSelectComboBox::TaoMultiSelectComboBox(QWidget* parent)
    : QComboBox(parent), d_ptr(new TaoMultiSelectComboBoxPrivate())
{
    Q_D(TaoMultiSelectComboBox);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_pExpandIconRotate = 0;
    d->_pExpandMarkWidth = 0;
    d->_themeMode = tTheme->getThemeMode();
    setFixedHeight(35);

    d->_comboBoxStyle = new TaoComboBoxStyle(style());
    setStyle(d->_comboBoxStyle);

    //调用view 让container初始化
    d->_comboView = new TaoComboBoxView(this);
    setView(d->_comboView);
    QAbstractItemView* comboBoxView = this->view();
    comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    TaoScrollBar* scrollBar = new TaoScrollBar(this);
    comboBoxView->setVerticalScrollBar(scrollBar);
    TaoScrollBar* floatVScrollBar = new TaoScrollBar(scrollBar, comboBoxView);
    floatVScrollBar->setIsAnimation(true);
    comboBoxView->setAutoScroll(false);
    comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
    comboBoxView->setSelectionBehavior(QAbstractItemView::SelectRows);
    comboBoxView->setObjectName("TaoComboBoxView");
    comboBoxView->setStyleSheet("#TaoComboBoxView{background-color:transparent;}");
    comboBoxView->setStyle(d->_comboBoxStyle);
    QWidget* container = this->findChild<QFrame*>();
    if (container)
    {
        container->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        container->setAttribute(Qt::WA_TranslucentBackground);
        container->setObjectName("TaoComboBoxContainer");
        container->setStyle(d->_comboBoxStyle);
        QLayout* layout = container->layout();
        while (layout->count())
        {
            layout->takeAt(0);
        }
        layout->addWidget(view());
        layout->setContentsMargins(6, 0, 6, 6);
#ifndef Q_OS_WIN
        container->setStyleSheet("background-color:transparent;");
#endif
    }
    QComboBox::setMaxVisibleItems(5);
    connect(d->_comboView, &TaoComboBoxView::itemPressed, d, &TaoMultiSelectComboBoxPrivate::onItemPressed);
    connect(this, QOverload<int>::of(&TaoMultiSelectComboBox::currentIndexChanged), d, &TaoMultiSelectComboBoxPrivate::_refreshCurrentIndexs);
    d->_itemSelection.resize(32);
    d->_itemSelection.fill(false);
    d->_itemSelection[0] = true;
    QComboBox::setMaxVisibleItems(5);
    connect(tTheme, &TaoTheme::themeModeChanged, this, [=](TaoThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

TaoMultiSelectComboBox::~TaoMultiSelectComboBox()
{
}

void TaoMultiSelectComboBox::setCurrentSelection(QString selection)
{
    Q_D(TaoMultiSelectComboBox);
    d->_itemSelection.fill(false);
    d->_comboView->selectionModel()->clearSelection();
    for (int i = 0; i < this->count(); i++)
    {
        if (selection == itemText(i))
        {
            QModelIndex index = model()->index(i, 0);
            d->_comboView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            d->_itemSelection[index.row()] = true;
        }
    }
    d->_refreshCurrentIndexs();
}

void TaoMultiSelectComboBox::setCurrentSelection(QStringList selection)
{
    Q_D(TaoMultiSelectComboBox);
    d->_comboView->selectionModel()->clearSelection();
    d->_itemSelection.fill(false);
    for (int i = 0; i < this->count(); i++)
    {
        if (selection.contains(itemText(i)))
        {
            QModelIndex index = model()->index(i, 0);
            d->_comboView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            d->_itemSelection[index.row()] = true;
        }
    }
    d->_refreshCurrentIndexs();
}

void TaoMultiSelectComboBox::setCurrentSelection(int index)
{
    Q_D(TaoMultiSelectComboBox);
    if (index >= this->count() || index < 0)
    {
        return;
    }
    d->_comboView->selectionModel()->clearSelection();
    d->_itemSelection.fill(false);
    QModelIndex currentIndex = model()->index(index, 0);
    d->_comboView->selectionModel()->select(currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    d->_itemSelection[index] = true;
    d->_refreshCurrentIndexs();
}

void TaoMultiSelectComboBox::setCurrentSelection(QList<int> selectionIndex)
{
    Q_D(TaoMultiSelectComboBox);
    d->_itemSelection.fill(false);
    d->_comboView->selectionModel()->clearSelection();
    for (auto index : selectionIndex)
    {
        if (index >= this->count() || index < 0)
        {
            continue;
        }
        QModelIndex currentIndex = model()->index(index, 0);
        d->_comboView->selectionModel()->select(currentIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        d->_itemSelection[index] = true;
    }
    d->_refreshCurrentIndexs();
}

QStringList TaoMultiSelectComboBox::getCurrentSelection() const
{
    return d_ptr->_selectedTextList;
}

QList<int> TaoMultiSelectComboBox::getCurrentSelectionIndex() const
{
    QList<int> indexList;
    for (int i = 0; i < d_ptr->_itemSelection.count(); i++)
    {
        if (d_ptr->_itemSelection[i])
        {
            indexList.append(i);
        }
    }
    return indexList;
}

void TaoMultiSelectComboBox::paintEvent(QPaintEvent* e)
{
    Q_D(TaoMultiSelectComboBox);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(TaoThemeColor(d->_themeMode, BasicBorder));
    painter.setBrush(isEnabled() ? underMouse() ? TaoThemeColor(d->_themeMode, BasicHover) : TaoThemeColor(d->_themeMode, BasicBase) : Qt::transparent);
    QRect foregroundRect = rect();
    foregroundRect.adjust(6, 1, -6, -1);
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    // 底边线绘制
    painter.setPen(TaoThemeColor(d->_themeMode, BasicBaseLine));
    painter.drawLine(foregroundRect.x() + d->_pBorderRadius, foregroundRect.y() + foregroundRect.height(), foregroundRect.x() + foregroundRect.width() - d->_pBorderRadius, foregroundRect.y() + foregroundRect.height());

    //文字绘制
    painter.setPen(isEnabled() ? TaoThemeColor(d->_themeMode, BasicText) : TaoThemeColor(d->_themeMode, BasicTextDisable));
    QString currentText = painter.fontMetrics().elidedText(d->_currentText, Qt::ElideRight, foregroundRect.width() - 27 - width() * 0.05);
    painter.drawText(15, height() / 2 + painter.fontMetrics().ascent() / 2 - 1, currentText);
    //展开指示器绘制
    painter.setPen(Qt::NoPen);
    painter.setBrush(d->_themeMode == TaoThemeType::Light ? QColor(0x0E, 0x6F, 0xC3) : QColor(0x4C, 0xA0, 0xE0));
    painter.drawRoundedRect(QRectF(width() / 2 - d->_pExpandMarkWidth, height() - 3, d->_pExpandMarkWidth * 2, 3), 2, 2);
    // 展开图标绘制
    if (count() > 0)
    {
        QFont iconFont = QFont("TaoAwesome");
        iconFont.setPixelSize(17);
        painter.setFont(iconFont);
        painter.setPen(isEnabled() ? TaoThemeColor(d->_themeMode, BasicText) : TaoThemeColor(d->_themeMode, BasicTextDisable));
        QRectF expandIconRect(width() - 25, 0, 20, height());
        painter.translate(expandIconRect.x() + (qreal)expandIconRect.width() / 2 - 2, expandIconRect.y() + (qreal)expandIconRect.height() / 2);
        painter.rotate(d->_pExpandIconRotate);
        painter.translate(-expandIconRect.x() - (qreal)expandIconRect.width() / 2 + 2, -expandIconRect.y() - (qreal)expandIconRect.height() / 2);
        painter.drawText(expandIconRect, Qt::AlignVCenter, QChar((unsigned short)TaoIconType::TAOfile));
        painter.restore();
    }
}

void TaoMultiSelectComboBox::showPopup()
{
    Q_D(TaoMultiSelectComboBox);
    bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo);
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
    QComboBox::showPopup();
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);

    if (count() > 0)
    {
        QWidget* container = this->findChild<QFrame*>();
        if (container)
        {
            int containerHeight = 0;
            if (count() >= maxVisibleItems())
            {
                containerHeight = maxVisibleItems() * 35 + 8;
            }
            else
            {
                containerHeight = count() * 35 + 8;
            }
            view()->resize(view()->width(), containerHeight - 8);
            container->move(container->x(), container->y() + 3);
            QLayout* layout = container->layout();
            while (layout->count())
            {
                layout->takeAt(0);
            }
            QPropertyAnimation* fixedSizeAnimation = new QPropertyAnimation(container, "maximumHeight");
            connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                container->setFixedHeight(value.toUInt());
            });
            fixedSizeAnimation->setStartValue(1);
            fixedSizeAnimation->setEndValue(containerHeight);
            fixedSizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
            fixedSizeAnimation->setDuration(400);
            fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(view(), "pos");
            connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_isAllowHidePopup = true;
                layout->addWidget(view());
            });
            QPoint viewPos = view()->pos();
            viewPosAnimation->setStartValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
            viewPosAnimation->setEndValue(viewPos);
            viewPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
            viewPosAnimation->setDuration(400);
            viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        //指示器动画
        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d, "pExpandIconRotate");
        connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(d->_pExpandIconRotate);
        rotateAnimation->setEndValue(-180);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_pExpandMarkWidth);
        qreal step = (width() / 2 - d->_pBorderRadius) / count();
        markAnimation->setEndValue(step * d->_selectedTextList.count());
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    d->_refreshCurrentIndexs();
}

void TaoMultiSelectComboBox::hidePopup()
{
    Q_D(TaoMultiSelectComboBox);
    if (d->_isFirstPopup && !this->view()->underMouse())
    {
        d->_isFirstPopup = false;
        return;
    }
    if (tApp->containsCursorToItem(d->_comboView))
    {
        return;
    }
    else
    {
        if (d->_isAllowHidePopup)
        {
            QWidget* container = this->findChild<QFrame*>();
            int containerHeight = container->height();
            if (container)
            {
                QLayout* layout = container->layout();
                while (layout->count())
                {
                    layout->takeAt(0);
                }
                QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(view(), "pos");
                connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() {
                    layout->addWidget(view());
                    QMouseEvent focusEvent(QEvent::MouseButtonPress, QPoint(-1, -1), QPoint(-1, -1), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
                    QApplication::sendEvent(parentWidget(), &focusEvent);
                    QComboBox::hidePopup();
                    container->setFixedHeight(containerHeight);
                });
                QPoint viewPos = view()->pos();
                connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() { view()->move(viewPos); });
                viewPosAnimation->setStartValue(viewPos);
                viewPosAnimation->setEndValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
                viewPosAnimation->setEasingCurve(QEasingCurve::InCubic);
                viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);

                QPropertyAnimation* fixedSizeAnimation = new QPropertyAnimation(container, "maximumHeight");
                connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                    container->setFixedHeight(value.toUInt());
                });
                fixedSizeAnimation->setStartValue(container->height());
                fixedSizeAnimation->setEndValue(1);
                fixedSizeAnimation->setEasingCurve(QEasingCurve::InCubic);
                fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
                d->_isAllowHidePopup = false;
            }
            //指示器动画
            QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d, "pExpandIconRotate");
            connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            rotateAnimation->setDuration(300);
            rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
            rotateAnimation->setStartValue(d->_pExpandIconRotate);
            rotateAnimation->setEndValue(0);
            rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
            markAnimation->setDuration(300);
            markAnimation->setEasingCurve(QEasingCurve::InOutSine);
            markAnimation->setStartValue(d->_pExpandMarkWidth);
            markAnimation->setEndValue(0);
            markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
    }
}
