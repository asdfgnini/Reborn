#ifndef TAOCOMBOBOXPRIVATE_H
#define TAOCOMBOBOXPRIVATE_H

#include <QObject>

#include "../../include/Def.h"
#include "../../include/stdafx.h"

class TaoComboBox;
class TaoComboBoxStyle;
class TaoComboBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(TaoComboBox);
    Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
    explicit TaoComboBoxPrivate(QObject* parent = nullptr);
    ~TaoComboBoxPrivate();

private:
    bool _isAllowHidePopup{false};
    TaoComboBoxStyle* _comboBoxStyle{nullptr};
    TaoThemeType::ThemeMode _themeMode;
};

#endif // TAOCOMBOBOXPRIVATE_H
