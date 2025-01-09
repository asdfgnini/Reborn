#ifndef TAOMULTISELECTCOMBOBOXPRIVATE_H
#define TAOMULTISELECTCOMBOBOXPRIVATE_H
#include <QObject>
#include <QVector>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoComboBoxStyle;
class TaoComboBoxView;
class TaoMultiSelectComboBox;
class TaoMultiSelectComboBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoMultiSelectComboBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE(qreal, ExpandIconRotate)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit TaoMultiSelectComboBoxPrivate(QObject* parent = nullptr);
    ~TaoMultiSelectComboBoxPrivate();
    Q_SLOT void onItemPressed(const QModelIndex& index);

private:
    TaoComboBoxStyle* _comboBoxStyle{nullptr};
    TaoComboBoxView* _comboView{nullptr};
    QVector<bool> _itemSelection;
    bool _isFirstPopup{false};
    bool _isAllowHidePopup{false};
    QString _currentText;
    QStringList _selectedTextList;
    void _refreshCurrentIndexs();
    void _adjustSelectedVector();
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOMULTISELECTCOMBOBOXPRIVATE_H
