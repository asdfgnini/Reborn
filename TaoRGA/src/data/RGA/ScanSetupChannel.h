#ifndef SCANSETUPCHANNEL_H
#define SCANSETUPCHANNEL_H

#include <QString>
#include <QMap>
#include <QMetaEnum>
#include <QVariant>
#include <QObject>
#include <QVector>
#include <QMutex>


#define SCAN_USER   0
#define SCAN_DEVICE 1
#define LEAK_USER   2
#define LEAK_DEVICE 3

namespace RGAType {
    Q_NAMESPACE

    // 枚举：通道模式
    enum ChannelMode {
        FixedNumber,       // 固定数字类型
        Time,               // 时间类型
        Timestamp,          // 时间戳类型
        SystemStatus,      // 系统状态类型
        Baseline,           // 基线类型
        TPQuad,             // TP四元类型
        TotalPressure,     // 总压力类型
        Single,             // 单一类型
        Sweep,              // 扫描类型
        Pressure,           // 压力类型
        EmissionCurrent,   // 发射电流类型
        AnodePotential,    // 阳极电位类型
        ElectronEnergy,    // 电子能量类型
        FocusPotential,    // 聚焦电位类型
        FilamentCurrent,    // 灯丝电流类型
        TotalPressureDetectorCurrent // 总压力检测电流类型
    };
    Q_ENUM_NS(ChannelMode)

    // 枚举：通道类型
    enum ChannelType {
        F32Value,           // 32位浮点数类型
        U32Value,           // 32位无符号整数类型
        
    };
    Q_ENUM_NS(ChannelType)
} // namespace RGAType

// 单个通道的配置结构
struct ChannelConfig {
    int id;
    bool enabled;
    RGAType::ChannelMode channelMode;
    double startMass;
    double stopMass;
    double ppamu;
    RGAType::ChannelType channelType;

    // 添加默认构造函数
    ChannelConfig() : id(0), enabled(false), 
                     channelMode(RGAType::ChannelMode::FixedNumber),
                     startMass(0.0), stopMass(0.0), ppamu(0.0),
                     channelType(RGAType::ChannelType::F32Value) {}
};

class ScanSetupChannel : public QObject {
    Q_OBJECT

public:
    static ScanSetupChannel* instance() {
        static ScanSetupChannel instance;
        return &instance;
    }

    // 获取指定设备的所有通道
    QVector<ChannelConfig> getAllChannels(int deviceId) const;
    
    // 获取指定设备的指定通道
    ChannelConfig getChannel(int deviceId, int channelId) const;
    
    // 更新指定设备的单个通道
    void updateChannel(int deviceId, const ChannelConfig& config);
    
    // 批量更新指定设备的通道
    void updateChannels(int deviceId, const QVector<ChannelConfig>& configs);
    
    // 设置指定设备的通道启用状态
    void setChannelEnabled(int deviceId, int channelId, bool enabled);
    
    // 清除指定设备的所有通道
    void clearChannels(int deviceId);
    
    // 获取指定设备的已启用通道数量
    int getEnabledChannelCount(int deviceId) const;
    
    // 打印指定设备的所有通道信息
    void printAllChannels(int deviceId) const;
    
    // 获取所有设备ID
    QList<int> getAllDeviceIds() const;
    
    // 检查设备是否存在通道配置
    bool hasDevice(int deviceId) const;
    
    // 检查设备的通道是否存在
    bool hasChannel(int deviceId, int channelId) const;
    
    // 通道模式和类型转换辅助函数
    static QString channelModeToString(RGAType::ChannelMode mode);
    static QString channelTypeToString(RGAType::ChannelType type);
    static RGAType::ChannelMode stringToChannelMode(const QString& modeStr);
    static RGAType::ChannelType stringToChannelType(const QString& typeStr);

signals:
    // 通道配置变化信号
    void channelConfigChanged(int deviceId, int channelId, const ChannelConfig& config);
    void channelsConfigChanged(int deviceId);
    void channelEnabledChanged(int deviceId, int channelId, bool enabled);
    void deviceChannelsCleared(int deviceId);

private:
    ScanSetupChannel(QObject* parent = nullptr) : QObject(parent) {}
    ~ScanSetupChannel() = default;
    
    ScanSetupChannel(const ScanSetupChannel&) = delete;
    ScanSetupChannel& operator=(const ScanSetupChannel&) = delete;

    QMap<int, QMap<int, ChannelConfig>> m_deviceChannels;
    mutable QMutex m_mutex;
};


#endif // SCANSETUPCHANNEL_H


// // 获取单例实例
// auto channels = ScanSetupChannel::instance();

// // 为设备1配置通道
// ChannelConfig config1;
// config1.id = 1;
// config1.enabled = true;
// config1.channelMode = RGAType::ChannelMode::TotalPressure;
// config1.startMass = 10.0;
// config1.stopMass = 20.0;
// config1.ppamu = 1.0;
// config1.channelType = RGAType::ChannelType::F32Value;

// channels->updateChannel(1, config1);

// // 为设备2配置多个通道
// QVector<ChannelConfig> configs;
// ChannelConfig config2;
// config2.id = 1;
// // ... 设置config2的其他属性 ...
// configs.append(config2);

// ChannelConfig config3;
// config3.id = 2;
// // ... 设置config3的其他属性 ...
// configs.append(config3);

// channels->updateChannels(2, configs);

// // 监听特定设备的通道变化
// connect(channels, &ScanSetupChannel::channelConfigChanged,
//         this, [](int deviceId, int channelId, const ChannelConfig& config) {
//     qDebug() << "Device" << deviceId << "Channel" << channelId << "updated";
// });

// // 打印所有设备的通道信息
// for (int deviceId : channels->getAllDeviceIds()) {
//     channels->printAllChannels(deviceId);
// }