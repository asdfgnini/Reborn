#include "ScanSetupChannel.h"
#include <QDebug>
#include <algorithm>

QVector<ChannelConfig> ScanSetupChannel::getAllChannels(int deviceId) const {
    QMutexLocker locker(&m_mutex);
    QVector<ChannelConfig> orderedChannels;
    
    if (!m_deviceChannels.contains(deviceId)) {
        qDebug() << "Device" << deviceId << "not found";
        return orderedChannels;
    }
    
    const auto& channels = m_deviceChannels[deviceId];
    QList<int> channelIds = channels.keys();
    std::sort(channelIds.begin(), channelIds.end());
    
    for (int id : channelIds) {
        orderedChannels.append(channels[id]);
    }
    
    return orderedChannels;
}

ChannelConfig ScanSetupChannel::getChannel(int deviceId, int channelId) const {
    QMutexLocker locker(&m_mutex);
    if (!m_deviceChannels.contains(deviceId)) {
        qDebug() << "设备" << deviceId << "未找到";
        return ChannelConfig();
    }
    const auto& channels = m_deviceChannels[deviceId];
    // qDebug() << "设备" << deviceId << "的通道数量:" << channels.size();
    // qDebug() << "查找通道ID:" << channelId;
    // qDebug() << "可用通道ID列表:" << channels.keys();
    
    auto config = m_deviceChannels[deviceId].value(channelId, ChannelConfig());

    // qDebug() << "找到的通道配置 - ID:" << config.id 
    //          << "启用状态:" << config.enabled
    //          << "模式:" << channelModeToString(config.channelMode);
             
    return config;
}

void ScanSetupChannel::updateChannel(int deviceId, const ChannelConfig& config) {
    QMutexLocker locker(&m_mutex);
    auto& channels = m_deviceChannels[deviceId];
    
    // 确保ID序列的连续性
    if (config.id <= 0 || (config.id > 1 && !channels.contains(config.id - 1))) {
        qWarning() << "Invalid channel ID sequence for device" << deviceId << ":" << config.id;
        return;
    }
    
    channels[config.id] = config;
    emit channelConfigChanged(deviceId, config.id, config);
}

void ScanSetupChannel::updateChannels(int deviceId, const QVector<ChannelConfig>& configs) {
    QMutexLocker locker(&m_mutex);
    bool hasUpdates = false;
    
    for (const auto& config : configs) {
        auto& channels = m_deviceChannels[deviceId];
        if (config.id <= 0 || (config.id > 1 && !channels.contains(config.id - 1))) {
            qWarning() << "Invalid channel ID sequence for device" << deviceId << ":" << config.id;
            continue;
        }
        
        channels[config.id] = config;
        hasUpdates = true;
        emit channelConfigChanged(deviceId, config.id, config);
    }
    
    if (hasUpdates) {
        emit channelsConfigChanged(deviceId);
    }
}

void ScanSetupChannel::setChannelEnabled(int deviceId, int channelId, bool enabled) {
    QMutexLocker locker(&m_mutex);
    if (!m_deviceChannels.contains(deviceId)) {
        qDebug() << "Device" << deviceId << "not found";
        return;
    }
    
    auto& channels = m_deviceChannels[deviceId];
    if (channels.contains(channelId)) {
        channels[channelId].enabled = enabled;
        emit channelEnabledChanged(deviceId, channelId, enabled);
        emit channelConfigChanged(deviceId, channelId, channels[channelId]);
    }
}

void ScanSetupChannel::clearChannels(int deviceId) {
    QMutexLocker locker(&m_mutex);
    if (m_deviceChannels.remove(deviceId) > 0) {
        emit deviceChannelsCleared(deviceId);
        emit channelsConfigChanged(deviceId);
    }
}

int ScanSetupChannel::getEnabledChannelCount(int deviceId) const {
    QMutexLocker locker(&m_mutex);
    if (!m_deviceChannels.contains(deviceId)) {
        return 0;
    }
    
    const auto& channels = m_deviceChannels[deviceId];
    int count = 0;
    
    QList<int> channelIds = channels.keys();
    std::sort(channelIds.begin(), channelIds.end());
    
    for (int id : channelIds) {
        if (channels[id].enabled) {
            count++;
        }
    }
    return count;
}

QList<int> ScanSetupChannel::getAllDeviceIds() const {
    QMutexLocker locker(&m_mutex);
    return m_deviceChannels.keys();
}

bool ScanSetupChannel::hasDevice(int deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_deviceChannels.contains(deviceId);
}

bool ScanSetupChannel::hasChannel(int deviceId, int channelId) const {
    QMutexLocker locker(&m_mutex);
    return m_deviceChannels.contains(deviceId) && 
           m_deviceChannels[deviceId].contains(channelId);
}

QString ScanSetupChannel::channelModeToString(RGAType::ChannelMode mode) {
    return QMetaEnum::fromType<RGAType::ChannelMode>().valueToKey(mode);
}

QString ScanSetupChannel::channelTypeToString(RGAType::ChannelType type) {
    return QMetaEnum::fromType<RGAType::ChannelType>().valueToKey(type);
}

RGAType::ChannelMode ScanSetupChannel::stringToChannelMode(const QString& modeStr) {
    QMetaEnum metaEnum = QMetaEnum::fromType<RGAType::ChannelMode>();
    bool ok;
    int value = metaEnum.keyToValue(modeStr.toLatin1().data(), &ok);
    if (!ok) {
        qDebug() << "Invalid ChannelMode string:" << modeStr;
        return RGAType::ChannelMode::FixedNumber; // 默认返回固定数字类型
    }
    return static_cast<RGAType::ChannelMode>(value);
}

RGAType::ChannelType ScanSetupChannel::stringToChannelType(const QString& typeStr) {
    QMetaEnum metaEnum = QMetaEnum::fromType<RGAType::ChannelType>();
    bool ok;
    int value = metaEnum.keyToValue(typeStr.toLatin1().data(), &ok);
    if (!ok) {
        qDebug() << "Invalid ChannelType string:" << typeStr;
        return RGAType::ChannelType::F32Value; // 默认返回 32位浮点数类型
    }
    return static_cast<RGAType::ChannelType>(value);
}

void ScanSetupChannel::printAllChannels(int deviceId) const {
    QMutexLocker locker(&m_mutex);
    
    // 添加调试信息
    qDebug() << "开始打印通道信息...";
    qDebug() << "设备ID:" << deviceId;
    qDebug() << "设备通道Map大小:" << m_deviceChannels.size();
    
    if (!m_deviceChannels.contains(deviceId)) {
        qDebug() << "设备" << deviceId << "未找到配置的通道";
        return;
    }
    
    try {
        const auto& channels = m_deviceChannels[deviceId];
        qDebug() << "\n=== 设备" << deviceId << "的通道配置详情 ===";
        qDebug() << "总通道数:" << channels.size();
        
        QList<int> channelIds = channels.keys();
        if (channelIds.isEmpty()) {
            qDebug() << "没有找到通道配置";
            return;
        }
        
        std::sort(channelIds.begin(), channelIds.end());
        
        for (int id : channelIds) {
            if (!channels.contains(id)) {
                qDebug() << "跳过无效通道ID:" << id;
                continue;
            }
            
            const auto& channel = channels[id];
            qDebug() << "\n通道" << channel.id << ":";
            qDebug() << "  启用状态:" << (channel.enabled ? "是" : "否");
            qDebug() << "  模式:" << channelModeToString(channel.channelMode);
            qDebug() << "  类型:" << channelTypeToString(channel.channelType);
            qDebug() << "  质量范围:" << channel.startMass << "到" << channel.stopMass;
            qDebug() << "  PPAMU:" << channel.ppamu;
        }
        
        qDebug() << "打印完成 ===\n";
    } catch (const std::exception& e) {
        qDebug() << "打印通道信息时发生异常:" << e.what();
    } catch (...) {
        qDebug() << "打印通道信息时发生未知异常";
    }
}