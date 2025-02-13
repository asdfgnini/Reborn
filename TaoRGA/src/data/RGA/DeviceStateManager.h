#pragma once

#include <QObject>
#include <QMap>
#include <QMutex>
#include <QDateTime>
#include <QString>
#include <QStringList>


#define DEVICE_ID0   "0"


// 定义捕获文件状态结构体
struct CaptureFileStatus {
    QString name;
    QString size;
};
//设备基本信息
struct DeviceBaseInfo {
    QString deviceId;
    uint16_t startChannel = 0;      //开始通道
    uint16_t stopChannel = 0;      //结束通道
    int16_t scanCount = 0;        //扫描次数
    uint32_t scanInterval = 0;     //扫描间隔
    uint16_t leakCheckMass = 0;    //漏检质量
    uint32_t scanTimeTotal = 0;    //扫描时间
    CaptureFileStatus captureFileStatus; //捕获文件状态


};
// http://192.168.1.101/mmsp/scanInfo/get
// DeviceStatusTypes.h
// 扫描状态
struct ScanStatus {
    QString deviceId;
    uint32_t firstScan = 0;
    uint32_t lastScan = 0;
    uint32_t pointsInCurrentScan = 0;
    uint32_t pointsPerScan = 0;
    uint32_t currentScan = 0;
    bool scanning = false;
};

// http://192.168.1.101/mmsp/communication/controlInfo/get
// 连接状态
struct ControlInfo {
    QString deviceId;
    bool amInControl = false;            //是否在控制
    bool canForceControl = false;        //是否可以强制控制
    bool canRequestControl = false;      //是否可以请求控制
    bool canTakeControl = false;         //是否可以接管控制
    bool controlled = false;             //是否被控制
    QString ipAddress;                   //IP地址
    bool locked = false;                  //是否锁定
    uint16_t secondsSinceLastControlRequest = 0; //上次控制请求时间
    uint16_t secondsSinceLastDataRequest = 0;    //上次数据请求时间
    uint16_t secondsSinceLastRequest = 0; //上次请求时间
};

// http://192.168.1.101/mmsp/measurement/get
// 测量状态
struct MeasurementStatus {
    QString deviceId;
    uint32_t nextScanNumber = 0;
    double totalPressure = 0.0;
};


// 传感器状态
struct SensorStatus {
    QString deviceId;
    double temperature = 0.0;
    double humidity = 0.0;
    double pressure = 0.0;
    bool isCalibrated = false;
};

// 系统状态
struct SystemStatus {
    QString deviceId;
    QString firmwareVersion;
    int batteryLevel = 0;
    bool isCharging = false;
    QString workMode;
};


// DeviceStateManager.h
class DeviceStateManager : public QObject {
    Q_OBJECT
public:
    // 状态类型枚举
    enum class StatusType {
        DeviceBaseInfo,
        Scan,
        Connection,
        Sensor,
        System
    };
    
    static DeviceStateManager* instance();
    
    // 更新特定类型的状态
    void updateDeviceBaseInfo(const QString& deviceId, const DeviceBaseInfo& status);
    void updateScanStatus(const QString& deviceId, const ScanStatus& status);
    void updateControlInfo(const QString& deviceId, const ControlInfo& status);
    void updateSensorStatus(const QString& deviceId, const SensorStatus& status);
    void updateSystemStatus(const QString& deviceId, const SystemStatus& status);
    void updateMeasurementStatus(const QString& deviceId, const MeasurementStatus& status);

    // 获取特定类型的状态
    ScanStatus getScanStatus(const QString& deviceId) const;
    ControlInfo getControlInfo(const QString& deviceId) const;
    SensorStatus getSensorStatus(const QString& deviceId) const;
    SystemStatus getSystemStatus(const QString& deviceId) const;
    DeviceBaseInfo getDeviceBaseInfo(const QString& deviceId) const;
    MeasurementStatus getMeasurementStatus(const QString& deviceId) const;

signals:
    // 不同类型状态变化的信号
    void scanStatusChanged(const QString& deviceId, const ScanStatus& status);
    void controlInfoChanged(const QString& deviceId, const ControlInfo& status);
    void sensorStatusChanged(const QString& deviceId, const SensorStatus& status);
    void systemStatusChanged(const QString& deviceId, const SystemStatus& status);
    void deviceBaseInfoChanged(const QString& deviceId, const DeviceBaseInfo& status);
    void measurementStatusChanged(const QString& deviceId, const MeasurementStatus& status);

private:
    mutable QMutex m_mutex;
    QMap<QString, ScanStatus> m_scanStatuses;
    QMap<QString, ControlInfo> m_controlInfoes;
    QMap<QString, SensorStatus> m_sensorStatuses;
    QMap<QString, SystemStatus> m_systemStatuses;
    QMap<QString, DeviceBaseInfo> m_deviceBaseInfo;
    QMap<QString, MeasurementStatus> m_measurementStatuses;

};



// // 在监控线程中更新状态
// void MonitorThread::processTask() {
//     while (shouldContinue()) {
//         // 更新扫描状态
//         ScanStatus scanStatus;
//         scanStatus.deviceId = "Device1";
//         scanStatus.isScanning = true;
//         scanStatus.scanProgress = 50;
//         scanStatus.lastScanTime = QDateTime::currentDateTime();
//         DeviceStateManager::instance()->updateScanStatus("Device1", scanStatus);

//         // 更新连接状态
//         ConnectionStatus connStatus;
//         connStatus.deviceId = "Device1";
//         connStatus.isConnected = true;
//         connStatus.signalStrength = 85;
//         DeviceStateManager::instance()->updateConnectionStatus("Device1", connStatus);

//         if (checkPaused()) continue;
//         QThread::msleep(100);
//     }
// }
