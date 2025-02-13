#include "RGADevice.h"

RGADevice::RGADevice(QWidget* parent)
    : m_deviceName("RBGAseries")

{
    m_scanMode = new T_ScanMode(parent);
    m_baseComponents = new T_BaseComponents(parent);
    m_rga = new RGA(parent);

    m_modeName.append(ModeType::ScanMode);
    m_modeName.append(ModeType::BaseComponents);

    //绑定连接按钮槽函数
    //用户点击连接按钮，将ip和端口写入urlgenerator方便生成url数据
    connect(m_scanMode,&T_ScanMode::scanmodeconnectclicked,m_rga,&RGA::startconnect);
    connect(m_rga,&RGA::connectisready,m_scanMode,&T_ScanMode::onconnectisready);

    //关闭连接
    connect(m_scanMode,&T_ScanMode::scanmodeconnectclickedclose,m_rga,&RGA::stopconnect);    
    connect(m_rga,&RGA::stopconnectisready,m_scanMode,&T_ScanMode::onconnectclickedclose);

    // //绑定扫描按钮的槽函数
    //开启
    //用户开启扫描
    connect(m_scanMode,&T_ScanMode::scanmodeclicked,m_rga,&RGA::startscan);
    connect(m_rga,&RGA::scanisready,m_scanMode,&T_ScanMode::onscanclicked);
    //用户关闭扫描
    connect(m_scanMode,&T_ScanMode::scanmodeclickedclose,m_rga,&RGA::stopscan);
    connect(m_rga,&RGA::stopscanisready,m_scanMode,&T_ScanMode::onscanclickedclose);

    //接收数据
    connect(m_rga,&RGA::dataReceived,m_scanMode,&T_ScanMode::onNewDataAvailable);

    //接收单位类型改变信号
    connect(m_scanMode,&T_ScanMode::unitMultiplierChanged,m_rga,&RGA::unitTypeChanged);
    connect(m_rga,&RGA::unitMultiplierChanged,m_scanMode,&T_ScanMode::onunitMultiplierChanged);

    //接收总压改变信号
    connect(m_rga,&RGA::totalPressureChanged,m_scanMode,&T_ScanMode::ontotalPressureChanged);
}

RGADevice::~RGADevice()
{

}

void RGADevice::setDeviceName(QString deviceName)
{
    m_deviceName = deviceName;
}

QString RGADevice::getDeviceName() const
{
    return m_deviceName;
}

void RGADevice::setm_scanMode(T_ScanMode* scanMode)
{
    m_scanMode = scanMode;
}

T_ScanMode* RGADevice::getm_scanMode() const
{
    return m_scanMode;
}

void RGADevice::setm_baseComponents(T_BaseComponents* baseComponents)
{
    m_baseComponents = baseComponents;
}

T_BaseComponents* RGADevice::getm_baseComponents() const
{
    return m_baseComponents;
}   

//返回除了modeName之外的modeName列表
QList<ModeType> RGADevice::getm_modeName(ModeType modeName) const
{
    QList<ModeType> modeNameList;
    for(int i = 0; i < m_modeName.size(); i++)
    {
        if(m_modeName[i] != modeName)
        {
            modeNameList.append(m_modeName[i]);
        }
    }
    return modeNameList;
}



