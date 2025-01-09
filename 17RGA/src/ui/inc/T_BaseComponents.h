#ifndef T_BASECOMPONENTS_H
#define T_BASECOMPONENTS_H

#include "T_BasePage.h"

class TaoToggleSwitch;
class TaoToggleButton;
class TaoComboBox;
class TaoMultiSelectComboBox;
class TaoMessageButton;
class TaoCheckBox;
class TaoSpinBox;
class TaoSlider;
class TaoRadioButton;
class TaoProgressBar;
class T_BaseComponents : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_BaseComponents(QWidget* parent = nullptr);
    ~T_BaseComponents();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    TaoToggleSwitch* _toggleSwitch{nullptr};
    TaoToggleButton* _toggleButton{nullptr};
    TaoComboBox* _comboBox{nullptr};
    TaoMultiSelectComboBox* _multiSelectComboBox{nullptr};
    TaoMessageButton* _messageButton{nullptr};
    TaoMessageButton* _infoMessageButton{nullptr};
    TaoMessageButton* _warningMessageButton{nullptr};
    TaoMessageButton* _errorMessageButton{nullptr};
    TaoCheckBox* _checkBox{nullptr};
    TaoSpinBox* _spinBox{nullptr};
    TaoSlider* _slider{nullptr};
    TaoRadioButton* _radioButton{nullptr};
    TaoProgressBar* _progressBar{nullptr};
};

#endif // T_BASECOMPONENTS_H
