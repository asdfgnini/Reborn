#ifndef T_SCANBASEPAGE_H
#define T_SCANBASEPAGE_H

#include <ElaScrollPage.h>
#include "../../../libs/ui/include/ElaToggleSwitch.h"
#include "../../../libs/ui/include/ElaLineEdit.h"
#include "../../../libs/ui/include/ElaComboBox.h"
#include "../../T_Home.h"

class QVBoxLayout;
class T_ScanBasePage : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit T_ScanBasePage(QWidget* parent = nullptr);
    ~T_ScanBasePage();

    void setConnectToggleButtonisdisable(bool isConnect);
    void setScanToggleButtonisdisable(bool isScan);
    void setm_startMass(int startMass);
    void setm_stopMass(int stopMass);
    void setm_ppamu(int ppamu);
    void setm_dwell(int dwell);
    //设置扫描数据是否禁用
    void setScanDataisDisable(bool isDisable);

    void setConnectButtonPressed(bool pressed) { isConnectButtonPressed = pressed; }
    void setScanButtonPressed(bool pressed) { isScanButtonPressed = pressed; }
    bool getConnectButtonPressed() { return isConnectButtonPressed; }
    bool getScanButtonPressed() { return isScanButtonPressed; }


    void setIsScanDevice(bool isScan) { isScanDevice = isScan; }
    bool getIsScanDevice() { return isScanDevice; }


    void setIsOpenFailed(bool isOpenFail) { isOpenFailed = isOpenFail; }
    bool getIsOpenFailed() { return isOpenFailed; }

    void setIsStopGetData(bool isStop) { isStopGetData = isStop; }
    bool getIsStopGetData() { return isStopGetData; }

    void setIsOtherUserClose(bool isOtherUser) { isOtherUserClose = isOtherUser; }
    bool getIsOtherUserClose() { return isOtherUserClose; }

    void setscanButtonState(bool isScan);
signals:



    //传递连接按钮的状态
    void connectclicked(QString ip, int port,bool checked);
    
    //传递连接按钮的状态
    void connectclickedclose(bool isscan);
    
    //传递扫描按钮的状态
    void saomiaoclicked(QString startMass,QString stopMass,QString ppamu,QString dwell,bool isUser);
    //传递扫描按钮的状态
    void saomiaoclickedclose(bool isUser);
    
public slots:


protected:
    void createCustomWidget(QString desText);
private:
    ElaToggleSwitch* connectToggleButton; 
    ElaToggleSwitch* ScanToggleButton;

    ElaLineEdit* AMULineEdit;
    ElaLineEdit* AMULineEdit2;
    ElaComboBox* fblcombox;
    ElaComboBox* timecombox;

    //是否是其他用户关闭
    bool isOtherUserClose;
    //是否停止获取数据
    bool isStopGetData;
    //是否是开启失败
    bool isOpenFailed;

    //与设备扫描状态一致
    bool isScanDevice;
    //前端按钮是否被按下
    bool isConnectButtonPressed;
    bool isScanButtonPressed;

};

#endif // T_BASEPAGE_H
