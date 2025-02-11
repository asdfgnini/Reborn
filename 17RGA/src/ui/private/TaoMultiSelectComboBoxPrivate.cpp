#include "TaoMultiSelectComboBoxPrivate.h"

#include <QPropertyAnimation>

#include "../inc/TaoComboBoxView.h"
#include "../inc/TaoMultiSelectComboBox.h"
TaoMultiSelectComboBoxPrivate::TaoMultiSelectComboBoxPrivate(QObject* parent)
    : QObject{parent}
{
}

TaoMultiSelectComboBoxPrivate::~TaoMultiSelectComboBoxPrivate()
{
}

void TaoMultiSelectComboBoxPrivate::onItemPressed(const QModelIndex& index)
{
    Q_Q(TaoMultiSelectComboBox);
    if (!_comboView->selectionModel()->isSelected(index))
    {
        _itemSelection[index.row()] = true;
    }
    else
    {
        _itemSelection[index.row()] = false;
    }
    _refreshCurrentIndexs();
    QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "pExpandMarkWidth");
    connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        q->update();
    });
    markAnimation->setDuration(300);
    markAnimation->setEasingCurve(QEasingCurve::InOutSine);
    markAnimation->setStartValue(_pExpandMarkWidth);
    qreal step = (q->width() / 2 - _pBorderRadius) / q->count();
    markAnimation->setEndValue(step * _selectedTextList.count());
    markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void TaoMultiSelectComboBoxPrivate::_refreshCurrentIndexs()
{
    Q_Q(TaoMultiSelectComboBox);
    QString str;
    _adjustSelectedVector();
    QVector<bool> selectedIndexVector;
    for (int i = 0; i < q->count(); i++)
    {
        // 该位选中
        QModelIndex index = q->model()->index(i, 0);
        if (_itemSelection[i])
        {
            _comboView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
            if (!str.isEmpty())
            {
                str.append(",");
            }
            str.append(q->itemText(i));
            selectedIndexVector.append(true);
        }
        else
        {
            _comboView->selectionModel()->select(index, QItemSelectionModel::Deselect);
            selectedIndexVector.append(false);
        }
    }
    if (str != _currentText)
    {
        q->update();
        _currentText = str;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        _selectedTextList = _currentText.split(",", Qt::SkipEmptyParts);
#else
        if (_selectedTextList.count() == 1 && _selectedTextList[0].isEmpty())
        {
            _selectedTextList.clear();
        }
#endif
        q->setCurrentIndex(-1);
        Q_EMIT q->itemSelectionChanged(selectedIndexVector);
        Q_EMIT q->currentTextListChanged(_selectedTextList);
    }
}

void TaoMultiSelectComboBoxPrivate::_adjustSelectedVector()
{
    Q_Q(TaoMultiSelectComboBox);
    while (_itemSelection.count() < q->count())
    {
        _itemSelection.append(false);
    }
}
