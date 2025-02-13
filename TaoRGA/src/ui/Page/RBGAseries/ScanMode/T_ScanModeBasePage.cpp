#include "T_ScanModeBasePage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../../../../include/ui.h"
#include "Def.h"
#include <qlogging.h>



T_ScanBasePage::T_ScanBasePage(QWidget* parent)
    : ElaScrollPage(parent)
    ,isScanDevice(false)
    ,isConnectButtonPressed(false)
    ,isScanButtonPressed(false)
    ,isOpenFailed(false)
    ,isStopGetData(false)
    
{
    ScanToggleButton = new ElaToggleSwitch(this);
    connectToggleButton = new ElaToggleSwitch(this);
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=]() {
        if (!parent)
        {
            update();
        }
    });

}

T_ScanBasePage::~T_ScanBasePage()
{

}

void T_ScanBasePage::createCustomWidget(QString desText)
{
    // 顶部元素
    QWidget* customWidget = new QWidget(this);

    ElaText* subTitleText = new ElaText(this);
    subTitleText->setText("IP地址");
    subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    subTitleText->setTextPixelSize(15);

    ElaText* ScanText = new ElaText(this);
    ScanText->setText("端口");
    ScanText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ScanText->setTextPixelSize(15);

    ElaText* toggleSwitchText = new ElaText("连接设备", this);
    toggleSwitchText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    toggleSwitchText->setTextPixelSize(20);
    


    ElaText* AMUText = new ElaText(this);
    AMUText->setText("质谱范围");
    AMUText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    AMUText->setTextPixelSize(15);

    ElaText* AMUText2 = new ElaText(this);
    AMUText2->setText("分辨率");
    AMUText2->setTextInteractionFlags(Qt::TextSelectableByMouse);
    AMUText2->setTextPixelSize(15);

    ElaText* AMUText3 = new ElaText(this);
    AMUText3->setText("扫描时间");
    AMUText3->setTextInteractionFlags(Qt::TextSelectableByMouse);
    AMUText3->setTextPixelSize(15);

    ElaText* scantoggleSwitchText = new ElaText("开始扫描", this);
    scantoggleSwitchText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    scantoggleSwitchText->setTextPixelSize(20);



    QHBoxLayout* TextLayout = new QHBoxLayout();
    TextLayout->setContentsMargins(0, 0, 0, 0);
    TextLayout->addWidget(subTitleText);
    TextLayout->addSpacing(110);
    TextLayout->addWidget(ScanText);
    TextLayout->addSpacing(50);
    TextLayout->addWidget(toggleSwitchText);
    TextLayout->addWidget(connectToggleButton);
    TextLayout->addStretch();
    TextLayout->addWidget(ScanToggleButton);      
    TextLayout->addWidget(scantoggleSwitchText);
    TextLayout->addSpacing(60);    
    TextLayout->addWidget(AMUText);
    TextLayout->addSpacing(100);
    TextLayout->addWidget(AMUText2);
    TextLayout->addSpacing(20);
    TextLayout->addWidget(AMUText3);
    TextLayout->addSpacing(10);





    //用于输入ip地址，目前只支持ipv4
    ElaLineEdit* ipLineEdit = new ElaLineEdit(this);
    ipLineEdit->setFixedSize(150,35);
    ipLineEdit->setPlaceholderText("IP:192.168.1.101");
    //设置缺省值
    ipLineEdit->setText("192.168.1.101");

    ElaLineEdit* portLineEdit = new ElaLineEdit(this);
    portLineEdit->setFixedSize(70,35);
    portLineEdit->setPlaceholderText("PORT:80");
    //设置缺省值
    portLineEdit->setText("80");

    AMULineEdit = new ElaLineEdit(this);
    AMULineEdit->setFixedSize(90,35);
    AMULineEdit->setPlaceholderText("0 Amu");
    //设置缺省值
    AMULineEdit->setText("1 Amu");

    ElaToolButton* iconbutton = new ElaToolButton(this);
    iconbutton->setElaIcon(ElaIconType::Minus);
    iconbutton->setFixedHeight(35);
    iconbutton->setEnabled(false);

    AMULineEdit2 = new ElaLineEdit(this);
    AMULineEdit2->setFixedSize(90,35);
    AMULineEdit2->setPlaceholderText("200 Amu");
    //设置缺省值
    AMULineEdit2->setText("50 Amu");

    fblcombox = new ElaComboBox(this);
    QStringList comboList{
        "1",
        "2",
        "5",
        "10"
                };
    fblcombox->addItems(comboList);
    fblcombox->setCurrentIndex(3);
    
    timecombox = new ElaComboBox(this);
    QStringList timecomboList{
        "8ms",
        "16ms",
        "32ms",
        "64ms",
        "128ms",
        "256ms",
        "512ms",
        "1024ms",
        "2048ms"
                };
    timecombox->addItems(timecomboList);
    timecombox->setCurrentIndex(2);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(ipLineEdit);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(portLineEdit);
    buttonLayout->addStretch();
    buttonLayout->addWidget(AMULineEdit);
    buttonLayout->addWidget(iconbutton);
    buttonLayout->addWidget(AMULineEdit2);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(fblcombox);
    buttonLayout->addSpacing(10);
    buttonLayout->addWidget(timecombox);


    QVBoxLayout* mainLayout  = new QVBoxLayout(customWidget);
    mainLayout ->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(TextLayout);
    mainLayout->addSpacing(5);
    mainLayout->addLayout(buttonLayout); // 例如左侧区域的其他控件

    setCustomWidget(customWidget);


    //绑定连接按钮的信号槽
    connect(connectToggleButton, &ElaToggleSwitch::toggled, this, [=](bool checked) 
    {
        if(checked == true)
        {
            if(getConnectButtonPressed() == false)
            {
                setConnectButtonPressed(true);
                ElaMessageBar::information(ElaMessageBarType::BottomLeft, "Information", "正在进行初始化!",3000);
                //禁用输入框
                ipLineEdit->setEnabled(false);
                portLineEdit->setEnabled(false);
                //获取ip信息
                QString ip = ipLineEdit->text();
                //获取端口信息
                int port = portLineEdit->text().toInt();
    #ifdef TAO_DEBUG         
                qDebug() << "ip:" << ip << "port:" << port;
    #endif
                //发送信号
                emit connectclicked(ip,port,checked);    
                connectToggleButton->setDisabled(true);
                ScanToggleButton->setDisabled(true);
            }
        }
        else 
        {
            if(getConnectButtonPressed() == true)
            {
                setConnectButtonPressed(false);
                ElaMessageBar::information(ElaMessageBarType::BottomLeft, "Information", "正在关闭连接!",3000);
                //恢复输入框状态
                ipLineEdit->setEnabled(true);
                //恢复输入框状态
                portLineEdit->setEnabled(true);

                connectToggleButton->setDisabled(true);
                ScanToggleButton->setDisabled(true);    

                if(getScanButtonPressed() == true)
                {
                    emit connectclickedclose(true);     
                }
                else
                {
                    emit connectclickedclose(false);
                }


            }                
        }
    });

    //绑定扫描按钮的信号槽
    connect(ScanToggleButton, &ElaToggleSwitch::toggled, this, [=](bool checked) {

        if(getIsScanDevice() == false)
        {
            //用户点击开始扫描按钮，且连接已建立，同时没有其他设备开启扫描
            if((checked == true) && (connectToggleButton->getIsToggled() == true))
            {
                qDebug() << "getIsScanDevice() == false && (checked == true) && (connectToggleButton->getIsToggled() == true)";
                if(getScanButtonPressed() == false)
                {
                    setScanButtonPressed(true);
                    ElaMessageBar::information(ElaMessageBarType::BottomLeft, "Information", "正在进行扫描初始化!",3000);
                    //获取扫描范围信息
                    QString startMass = AMULineEdit->text();
                    startMass.remove(" Amu");
                    QString stopMass = AMULineEdit2->text();
                    stopMass.remove(" Amu");

                    //获取分辨率信息
                    QString ppamu = fblcombox->currentText();
                    
                    //获取扫描时间信息
                    QString dwell = timecombox->currentText();
                    dwell.remove("ms");
                    //发送信号
        #ifdef TAO_DEBUG         
                    qDebug() << "amu:" << startMass << "stopMass:" << stopMass << "ppamu:" << ppamu << "dwell:" << dwell;
        #endif
                    //不能修改值
                    AMULineEdit->setEnabled(false);
                    AMULineEdit2->setEnabled(false);
                    fblcombox->setEnabled(false);
                    timecombox->setEnabled(false);
                    //标识扫描按钮按下
                    setScanButtonPressed(true);

                    
                    //发送信号
                    emit saomiaoclicked(startMass,stopMass,ppamu,dwell,true);            
                    ScanToggleButton->setDisabled(true);
                    connectToggleButton->setDisabled(true);
                }
            }
            else if ((checked == true) && (connectToggleButton->getIsToggled() == false))//没有建立连接 
            {
                ScanToggleButton->setIsToggled(false);
            }
            //用户点击了关闭扫描按钮，恢复输入框状态
            if(checked == false)
            {
                if(getScanButtonPressed() == true)
                {
                    //标识扫描按钮关闭
                    setScanButtonPressed(false);
                    AMULineEdit->setEnabled(true);
                    AMULineEdit2->setEnabled(true);
                    fblcombox->setEnabled(true);
                    timecombox->setEnabled(true);
                    qDebug() << "getIsOpenFailed() == false &&getIsStopGetData() == false";
                    if(getIsOpenFailed() == false &&getIsStopGetData() == false && getIsOtherUserClose() == false)
                    {
                        ElaMessageBar::information(ElaMessageBarType::BottomLeft, "Information", "正在关闭扫描!",3000);
                        //禁止用户操作
                        ScanToggleButton->setDisabled(true);
                        connectToggleButton->setDisabled(true);    
                        qDebug() << "emit saomiaoclickedclose(checked).................................";                
                        emit saomiaoclickedclose(checked);              
                    }
                    else
                    {
                        setIsOtherUserClose(false);
                        setIsStopGetData(false);
                        setIsOpenFailed(false);
                    }


                }
            }
        }
        //如果设备已经开始扫描，用户关闭扫描按钮
        if(getIsScanDevice() == true && checked == false)
        {
            qDebug() << "getIsScanDevice() == true && checked == false";
            if(getScanButtonPressed() == true)
            {
                setScanButtonPressed(false);
                setIsScanDevice(false);
                AMULineEdit->setEnabled(true);
                AMULineEdit2->setEnabled(true);
                fblcombox->setEnabled(true);
                timecombox->setEnabled(true);
                ScanToggleButton->setDisabled(true);
                connectToggleButton->setDisabled(true);                    
                emit saomiaoclickedclose(checked);
            }
        }
    });
}   

void T_ScanBasePage::setConnectToggleButtonisdisable(bool isConnect)
{
    connectToggleButton->setDisabled(isConnect);
}

void T_ScanBasePage::setScanToggleButtonisdisable(bool isScan)
{
    ScanToggleButton->setDisabled(isScan);
}

void T_ScanBasePage::setm_startMass(int startMass)
{
    AMULineEdit->setText(QString::number(startMass) + " Amu");
}

void T_ScanBasePage::setm_stopMass(int stopMass)
{
    AMULineEdit2->setText(QString::number(stopMass) + " Amu");
}

void T_ScanBasePage::setm_ppamu(int ppamu)
{
    if(ppamu == 1)
    {
        fblcombox->setCurrentIndex(0);
    }
    else if(ppamu == 2)
    {
        fblcombox->setCurrentIndex(1);
    }
    else if(ppamu == 5)
    {
        fblcombox->setCurrentIndex(2);
    }
    else if(ppamu == 10)
    {
        fblcombox->setCurrentIndex(3);
    }
}

void T_ScanBasePage::setm_dwell(int dwell)
{       
    if(dwell == 8)
    {
        timecombox->setCurrentIndex(0);
    }
    else if(dwell == 16)
    {
        timecombox->setCurrentIndex(1);
    }
    else if(dwell == 32)
    {
        timecombox->setCurrentIndex(2);
    }
    else if(dwell == 64)
    {
        timecombox->setCurrentIndex(3);
    }
    else if(dwell == 128)
    {
        timecombox->setCurrentIndex(4);
    }
    else if(dwell == 256)
    {
        timecombox->setCurrentIndex(5);
    }
    else if(dwell == 512)
    {
        timecombox->setCurrentIndex(6);
    }
    else if(dwell == 1024)
    {
        timecombox->setCurrentIndex(7);
    }
    else if(dwell == 2048)
    {
        timecombox->setCurrentIndex(8);
    }
}

void T_ScanBasePage::setscanButtonState(bool isScan)
{
    ScanToggleButton->setIsToggled(isScan);
}

void T_ScanBasePage::setScanDataisDisable(bool isDisable)
{
    AMULineEdit->setDisabled(isDisable);
    AMULineEdit2->setDisabled(isDisable);
    fblcombox->setDisabled(isDisable);
    timecombox->setDisabled(isDisable);
}