#ifndef TAOMULTISELECTCOMBOBOX_H
#define TAOMULTISELECTCOMBOBOX_H
#include <QComboBox>

#include "../../include/stdafx.h"
class TaoMultiSelectComboBoxPrivate;
class TAO_EXPORT TaoMultiSelectComboBox : public QComboBox
{
    Q_OBJECT
    Q_Q_CREATE(TaoMultiSelectComboBox)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
public:
    explicit TaoMultiSelectComboBox(QWidget* parent = nullptr);
    ~TaoMultiSelectComboBox();
    void setCurrentSelection(QString selection);
    void setCurrentSelection(QStringList selection);
    void setCurrentSelection(int index);
    void setCurrentSelection(QList<int> selectionIndex);
    QStringList getCurrentSelection() const;
    QList<int> getCurrentSelectionIndex() const;
Q_SIGNALS:
    Q_SIGNAL void itemSelectionChanged(QVector<bool> itemSelection);
    Q_SIGNAL void currentTextListChanged(QStringList selectedTextList);

protected:
    void paintEvent(QPaintEvent* e) override;
    void showPopup() override;
    void hidePopup() override;
};

#endif // TAOMULTISELECTCOMBOBOX_H
