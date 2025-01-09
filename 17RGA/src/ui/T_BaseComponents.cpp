#include "./inc/T_BaseComponents.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "./inc/TaoCheckBox.h"
#include "./inc/TaoComboBox.h"
#include "./inc/TaoMessageButton.h"
#include "./inc/TaoMultiSelectComboBox.h"
#include "./inc/TaoPlainTextEdit.h"
#include "./inc/TaoProgressBar.h"
#include "./inc/TaoRadioButton.h"
#include "./inc/TaoScrollPageArea.h"
#include "./inc/TaoSlider.h"
#include "./inc/TaoSpinBox.h"
#include "./inc/TaoText.h"
#include "./inc/TaoToggleButton.h"
#include "./inc/TaoToggleSwitch.h"

T_BaseComponents::T_BaseComponents(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("TaoBaseComponents");

    // 顶部元素
    createCustomWidget("一些常用的基础组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    _toggleSwitch = new TaoToggleSwitch(this);
    TaoScrollPageArea* toggleSwitchArea = new TaoScrollPageArea(this);
    QHBoxLayout* toggleSwitchLayout = new QHBoxLayout(toggleSwitchArea);
    TaoText* toggleSwitchText = new TaoText("TaoToggleSwitch", this);
    toggleSwitchText->setTextPixelSize(15);
    toggleSwitchLayout->addWidget(toggleSwitchText);
    toggleSwitchLayout->addWidget(_toggleSwitch);
    toggleSwitchLayout->addStretch();
    TaoToggleSwitch* toggleSwitchDisableSwitch = new TaoToggleSwitch(this);
    TaoText* toggleSwitchDisableText = new TaoText("禁用", this);
    toggleSwitchDisableText->setTextPixelSize(15);
    connect(toggleSwitchDisableSwitch, &TaoToggleSwitch::toggled, this, [=](bool checked) {
        _toggleSwitch->setDisabled(checked);
    });
    toggleSwitchLayout->addWidget(toggleSwitchDisableSwitch);
    toggleSwitchLayout->addWidget(toggleSwitchDisableText);
    toggleSwitchLayout->addSpacing(10);

    _toggleButton = new TaoToggleButton("ToggleButton", this);
    _toggleButton->setFixedWidth(120);
    TaoScrollPageArea* toggleButtonArea = new TaoScrollPageArea(this);
    QHBoxLayout* toggleButtonLayout = new QHBoxLayout(toggleButtonArea);
    TaoText* toggleButtonText = new TaoText("TaoToggleButton", this);
    toggleButtonText->setTextPixelSize(15);
    toggleButtonLayout->addWidget(toggleButtonText);
    toggleButtonLayout->addWidget(_toggleButton);
    toggleButtonLayout->addStretch();
    TaoToggleSwitch* toggleButtonDisableSwitch = new TaoToggleSwitch(this);
    TaoText* toggleButtonDisableText = new TaoText("禁用", this);
    toggleButtonDisableText->setTextPixelSize(15);
    connect(toggleButtonDisableSwitch, &TaoToggleSwitch::toggled, this, [=](bool checked) {
        _toggleButton->setDisabled(checked);
    });
    toggleButtonLayout->addWidget(toggleButtonDisableSwitch);
    toggleButtonLayout->addWidget(toggleButtonDisableText);
    toggleButtonLayout->addSpacing(10);

    _comboBox = new TaoComboBox(this);
    QStringList comboList{
        "我愿投身前途未卜的群星",
        "潜行 步伐小心翼翼",
        "不留游走痕迹",
        "如同一簇幽灵",
        "所谓 道德加上伦理",
        "抱歉只能律己"};
    _comboBox->addItems(comboList);
    TaoScrollPageArea* comboBoxArea = new TaoScrollPageArea(this);
    QHBoxLayout* comboBoxLayout = new QHBoxLayout(comboBoxArea);
    TaoText* comboBoxText = new TaoText("TaoComboBox", this);
    comboBoxText->setTextPixelSize(15);
    comboBoxLayout->addWidget(comboBoxText);
    comboBoxLayout->addWidget(_comboBox);
    comboBoxLayout->addStretch();
    TaoToggleSwitch* comboBoxDisableSwitch = new TaoToggleSwitch(this);
    TaoText* comboBoxDisableText = new TaoText("禁用", this);
    comboBoxDisableText->setTextPixelSize(15);
    connect(comboBoxDisableSwitch, &TaoToggleSwitch::toggled, this, [=](bool checked) {
        _comboBox->setDisabled(checked);
    });
    comboBoxLayout->addWidget(comboBoxDisableSwitch);
    comboBoxLayout->addWidget(comboBoxDisableText);
    comboBoxLayout->addSpacing(10);

    _multiSelectComboBox = new TaoMultiSelectComboBox(this);
    QStringList multiComboList{"执念的鱼", "提着灯闯过远洋的甄选", "继续下潜", "无需誓言", "我的心像自沉的旧母舰", "没入深渊", "我曾凝望曾是航向的日出"};
    QStringList multiSelectComboList{"执念的鱼", "提着灯闯过远洋的甄选", "无需誓言", "我的心像自沉的旧母舰"};
    _multiSelectComboBox->addItems(multiComboList);
    _multiSelectComboBox->setCurrentSelection(multiSelectComboList);
    TaoScrollPageArea* multiSelectComboBoxArea = new TaoScrollPageArea(this);
    QHBoxLayout* multiSelectComboBoxLayout = new QHBoxLayout(multiSelectComboBoxArea);
    TaoText* multiSelectComboBoxText = new TaoText("TaoMutilSelectComboBox", this);
    multiSelectComboBoxText->setTextPixelSize(15);
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxText);
    multiSelectComboBoxLayout->addWidget(_multiSelectComboBox);
    multiSelectComboBoxLayout->addStretch();
    TaoToggleSwitch* multiSelectComboBoxDisableSwitch = new TaoToggleSwitch(this);
    TaoText* multiSelectComboBoxDisableText = new TaoText("禁用", this);
    multiSelectComboBoxDisableText->setTextPixelSize(15);
    connect(multiSelectComboBoxDisableSwitch, &TaoToggleSwitch::toggled, this, [=](bool checked) {
        _multiSelectComboBox->setDisabled(checked);
    });
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxDisableSwitch);
    multiSelectComboBoxLayout->addWidget(multiSelectComboBoxDisableText);
    multiSelectComboBoxLayout->addSpacing(10);

    _messageButton = new TaoMessageButton("Success", this);
    _messageButton->setBarTitle("Success");
    _messageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");

    _infoMessageButton = new TaoMessageButton("Info", this);
    _infoMessageButton->setBarTitle("Information");
    _infoMessageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _infoMessageButton->setMessageMode(TaoMessageBarType::Information);
    _infoMessageButton->setPositionPolicy(TaoMessageBarType::TopLeft);

    _warningMessageButton = new TaoMessageButton("Warning", this);
    _warningMessageButton->setBarTitle("Warning");
    _warningMessageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _warningMessageButton->setMessageMode(TaoMessageBarType::Warning);
    _warningMessageButton->setPositionPolicy(TaoMessageBarType::BottomLeft);

    _errorMessageButton = new TaoMessageButton("Error", this);
    _errorMessageButton->setBarTitle("Error");
    _errorMessageButton->setBarText("点燃星 亲手点燃黑暗森林的火星 蒙昧初醒 而我却 轻声告别这新生的黎明");
    _errorMessageButton->setMessageMode(TaoMessageBarType::Error);
    _errorMessageButton->setPositionPolicy(TaoMessageBarType::BottomRight);

    TaoScrollPageArea* messageButtonArea = new TaoScrollPageArea(this);
    QHBoxLayout* messageButtonLayout = new QHBoxLayout(messageButtonArea);
    TaoText* messageButtonText = new TaoText("TaoMessageButton", this);
    messageButtonText->setTextPixelSize(15);
    messageButtonLayout->addWidget(messageButtonText);
    messageButtonLayout->addWidget(_messageButton);
    messageButtonLayout->addWidget(_infoMessageButton);
    messageButtonLayout->addWidget(_warningMessageButton);
    messageButtonLayout->addWidget(_errorMessageButton);
    messageButtonLayout->addStretch();
    TaoToggleSwitch* messageButtonDisableSwitch = new TaoToggleSwitch(this);
    TaoText* messageButtonDisableText = new TaoText("禁用", this);
    messageButtonDisableText->setTextPixelSize(15);
    connect(messageButtonDisableSwitch, &TaoToggleSwitch::toggled, this, [=](bool checked) {
        _messageButton->setDisabled(checked);
        _infoMessageButton->setDisabled(checked);
        _warningMessageButton->setDisabled(checked);
        _errorMessageButton->setDisabled(checked);
    });
    messageButtonLayout->addWidget(messageButtonDisableSwitch);
    messageButtonLayout->addWidget(messageButtonDisableText);
    messageButtonLayout->addSpacing(10);

    _checkBox = new TaoCheckBox("CheckBox", this);
    TaoScrollPageArea* checkBoxArea = new TaoScrollPageArea(this);
    QHBoxLayout* checkBoxLayout = new QHBoxLayout(checkBoxArea);
    TaoText* checkBoxText = new TaoText("TaocheckBox", this);
    checkBoxText->setTextPixelSize(15);
    checkBoxLayout->addWidget(checkBoxText);
    checkBoxLayout->addWidget(_checkBox);
    checkBoxLayout->addStretch();
    TaoToggleSwitch* checkBoxDisableSwitch = new TaoToggleSwitch(this);
    TaoText* checkBoxDisableText = new TaoText("禁用", this);
    checkBoxDisableText->setTextPixelSize(15);
    connect(checkBoxDisableSwitch, &TaoToggleSwitch::toggled, this, [=](bool checked) {
        _checkBox->setDisabled(checked);
    });
    checkBoxLayout->addWidget(checkBoxDisableSwitch);
    checkBoxLayout->addWidget(checkBoxDisableText);
    checkBoxLayout->addSpacing(10);

    _spinBox = new TaoSpinBox(this);
    TaoScrollPageArea* spinBoxArea = new TaoScrollPageArea(this);
    QHBoxLayout* spinBoxLayout = new QHBoxLayout(spinBoxArea);
    TaoText* spinBoxText = new TaoText("TaoSpinBox", this);
    spinBoxText->setTextPixelSize(15);
    spinBoxLayout->addWidget(spinBoxText);
    spinBoxLayout->addWidget(_spinBox);
    spinBoxLayout->addStretch();

    _slider = new TaoSlider(this);
    TaoScrollPageArea* sliderArea = new TaoScrollPageArea(this);
    QHBoxLayout* sliderLayout = new QHBoxLayout(sliderArea);
    TaoText* sliderText = new TaoText("TaoSlider", this);
    sliderText->setTextPixelSize(15);
    sliderLayout->addWidget(sliderText);
    sliderLayout->addWidget(_slider);
    sliderLayout->addStretch();

    _radioButton = new TaoRadioButton("RadioButton", this);
    TaoScrollPageArea* radioButtonArea = new TaoScrollPageArea(this);
    QHBoxLayout* radioButtonLayout = new QHBoxLayout(radioButtonArea);
    TaoText* radioButtonText = new TaoText("TaoRadioButton", this);
    radioButtonText->setTextPixelSize(15);
    radioButtonLayout->addWidget(radioButtonText);
    radioButtonLayout->addWidget(_radioButton);
    radioButtonLayout->addStretch();

    _progressBar = new TaoProgressBar(this);
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0);
    TaoScrollPageArea* progressBarArea = new TaoScrollPageArea(this);
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    TaoText* progressBarText = new TaoText("TaoProgressBar", this);
    progressBarText->setTextPixelSize(15);
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);
    progressBarLayout->addStretch();

    TaoPlainTextEdit* edit = new TaoPlainTextEdit(this);
    edit->setPlainText("这是一个TaoPlainTextEdit  暂时放在这里");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("TaoBaseComponents");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addWidget(toggleSwitchArea);
    centerLayout->addWidget(toggleButtonArea);
    centerLayout->addWidget(comboBoxArea);
    centerLayout->addWidget(multiSelectComboBoxArea);
    centerLayout->addWidget(messageButtonArea);
    centerLayout->addWidget(checkBoxArea);
    centerLayout->addWidget(spinBoxArea);
    centerLayout->addWidget(sliderArea);
    centerLayout->addWidget(radioButtonArea);
    centerLayout->addWidget(progressBarArea);
    centerLayout->addWidget(edit);
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);

    TaoText* homeStack1 = new TaoText("HomeStack1", this);
    QFont font = homeStack1->font();
    font.setPixelSize(32);
    homeStack1->setFont(font);
    homeStack1->setAlignment(Qt::AlignCenter);
    homeStack1->setWindowTitle("HomeStack1");
    addCentralWidget(homeStack1);
    TaoText* homeStack2 = new TaoText("HomeStack2", this);
    homeStack2->setFont(font);
    homeStack2->setAlignment(Qt::AlignCenter);
    homeStack2->setWindowTitle("HomeStack2");
    addCentralWidget(homeStack2);
}

T_BaseComponents::~T_BaseComponents()
{
}

void T_BaseComponents::mouseReleaseEvent(QMouseEvent* event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
    {
        //TaoMessageBar::success(TaoMessageBarType::TopRight, "Success", "Never Close Your Eyes", 2500);
        //TaoMessageBar::success(TaoMessageBarType::TopRight, "Success", "Never Close Your Eyes", 1500);
        break;
    }
    case Qt::BackButton:
    {
        this->navigation(0);
        break;
    }
    case Qt::ForwardButton:
    {
        this->navigation(1);
        break;
    }
    case Qt::MiddleButton:
    {
        this->navigation(2);
        break;
    }
    default:
    {
        break;
    }
    }
    TaoScrollPage::mouseReleaseEvent(event);
}
