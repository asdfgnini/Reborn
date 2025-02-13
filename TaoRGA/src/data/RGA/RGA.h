#pragma once

#include <QObject>

#include "UrlGenerator.h"
#include "DeviceMonitor.h"
#include "DataFetcher.h"
#include "ScanSetupChannel.h"
#include "DeviceStateManager.h"



class RGA : public QObject
{
    Q_OBJECT
public:
    RGA(QObject *parent = nullptr );
    ~RGA();
public slots:
    // 给外界调用，启用URL生成线程,主要响应 连接 按钮的信号
    void startconnect(QString ip, int port,bool checked);
    void stopconnect(bool isscan);
    // 给外界调用，启用URL生成线程,主要响应 扫描 按钮的信号
    void startscan(QString startMass,QString stopMass,QString ppamu,QString dwell,bool scanstate);
    void stopscan(bool scanstate);
    // 单位类型改变槽函数，用于接收后端返回的单位类型乘数，然后更新单位类型乘数
    void unitTypeChanged(QString unitType);
private slots:
//连接相关
    void ondataUpdated();
    void ondeviceBaseInfoReady();
    void onAllChannelsInfoReady();
    void onstopconnect();
//扫描相关
    void onDataScanUpdated(QString startMass, QString stopMass, QString ppamu, QString dwell);
    void onchannelsetisready(QList<QString> failedUrls);
    void onscanStatusChanged(const QString& deviceId, const ScanStatus& status);
    void oncontrolInfoChanged(const QString& deviceId, const ControlInfo& info);
    void onstopscanSignal(bool iscan);
//监控测量状态
    void onmeasurementStatusChanged(const QString& deviceId, const MeasurementStatus& status);
signals:
    //用于发送数据到前端
    void dataReceived(QVariantMap data);
    //
    void deviceBaseInfoReady();
    void allChannelsInfoReady(); 
    void connectisready(bool isscanning,int startMass,int stopMass,int ppamu,int dwell);
    void stopconnectisready();
    void channelsetisready(QList<QString> failedUrls);
    void scanisready(bool isready);
    void stopscanisready(bool iscan);
    void unitMultiplierChanged(double unitMultiplier);
    void totalPressureChanged(double totalPressure);

private:
    //解析单位类型转换所需要的数据
    void parseunitMultiplier(const QString& response);
    //解析设备基本信息
    void parseDeviceBaseInfo(const QString& response);

    //解析通道信息
    void parseChannelInfo(const QString& response,int channelId);

    //添加一个通用的请求方法
    void executeUrlRequestsonce(const RequestFilter& filter, std::function<void(const QString&)> successCallback = nullptr);    
    void executeUrlRequestsoncesort(const RequestFilter& filter, std::function<void(const QString&)> successCallback = nullptr);
    //同步请求
    bool HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, int timeout, ResponseFormat format);
    // 添加一个辅助函数来解析通道模式
    RGAType::ChannelMode parseChannelMode(const QString& modeStr);
    void executeUrlRequestsBatch(const QVector<std::pair<RequestFilter, int>>& requests, 
                               std::function<void(const QString&, int)> successCallback);
    void UserChannelConfig(int startMass,int stopMass,int ppamu);

    //保存一份设备基本信息
    DeviceBaseInfo m_deviceBaseInfo;
    //保存一份通道配置
    QVector<ChannelConfig> m_channelConfigs;
    //保存设备当前的扫描设置
    int deviceStartMass;
    int deviceStopMass;
    int devicePpamu;
    int deviceDwell;
    //保存用户的扫描设置
    int m_startMass;
    int m_stopMass;
    int m_ppamu;

    //保存一份用户设置的ip和端口
    QString m_ip;
    int m_port;

    //扫描状态，与界面按钮状态一致
    bool m_scanstate;

    //单位转换所需要的数据保存
    double Sensitivity;//敏感度因子
    double FragFactor;//碎片因子
    double IonizationProbability;//电离概率
    double unitMultiplier = 1;//单位转换倍数

    //扫描状态与设备状态一致，这个表示当前设备状态
    bool m_currentscanstate_device;
    //扫描状态与设备状态一致，这个表示上次设备状态
    bool m_lastscanstate_device;

    //保存当前的测量状态
    double m_currenttotalpressure;


    //保存当前的控制信息
    ControlInfo m_currentcontrolinfo;
    //保存上一次的控制信息
    ControlInfo m_lastcontrolinfo;


    mutable QMutex m_mutex;
    //用于生成url
    UrlGenerator* m_urlGenerator;
    //用于监控设备状态

    DeviceMonitor* m_deviceMonitor;
    //用于获取数据
    DataFetcher* m_dataFetcher;

};
