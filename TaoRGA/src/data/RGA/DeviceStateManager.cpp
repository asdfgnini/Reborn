#include "DeviceStateManager.h"
#include <QApplication>
#include <QThread>

DeviceStateManager* DeviceStateManager::instance() {
    if (QThread::currentThread() != qApp->thread()) {
        // 如果在子线程中首次访问，强制在主线程中创建实例
        QMetaObject::invokeMethod(qApp, []() {
            DeviceStateManager::instance();  // 在主线程中创建实例
        }, Qt::BlockingQueuedConnection);
    }
    
    static DeviceStateManager instance;
    return &instance;
}

void DeviceStateManager::updateScanStatus(const QString& deviceId, const ScanStatus& status) {
    QMutexLocker locker(&m_mutex);
    m_scanStatuses[deviceId] = status;
    emit scanStatusChanged(deviceId, status);
}

void DeviceStateManager::updateControlInfo(const QString& deviceId, const ControlInfo& status) {
    QMutexLocker locker(&m_mutex);
    m_controlInfoes[deviceId] = status;
    emit controlInfoChanged(deviceId, status);
}

void DeviceStateManager::updateSensorStatus(const QString& deviceId, const SensorStatus& status) {
    QMutexLocker locker(&m_mutex);
    m_sensorStatuses[deviceId] = status;
    emit sensorStatusChanged(deviceId, status);
}

void DeviceStateManager::updateSystemStatus(const QString& deviceId, const SystemStatus& status) {
    QMutexLocker locker(&m_mutex);
    m_systemStatuses[deviceId] = status;
    emit systemStatusChanged(deviceId, status);
}

ScanStatus DeviceStateManager::getScanStatus(const QString& deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_scanStatuses.value(deviceId);
}

ControlInfo DeviceStateManager::getControlInfo(const QString& deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_controlInfoes.value(deviceId);
}


SensorStatus DeviceStateManager::getSensorStatus(const QString& deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_sensorStatuses.value(deviceId);
}

SystemStatus DeviceStateManager::getSystemStatus(const QString& deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_systemStatuses.value(deviceId);
}

DeviceBaseInfo DeviceStateManager::getDeviceBaseInfo(const QString& deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_deviceBaseInfo.value(deviceId);
}

void DeviceStateManager::updateDeviceBaseInfo(const QString& deviceId, const DeviceBaseInfo& status) {
    QMutexLocker locker(&m_mutex);
    m_deviceBaseInfo[deviceId] = status;
    emit deviceBaseInfoChanged(deviceId, status);
}

void DeviceStateManager::updateMeasurementStatus(const QString& deviceId, const MeasurementStatus& status) {
    QMutexLocker locker(&m_mutex);
    m_measurementStatuses[deviceId] = status;
    emit measurementStatusChanged(deviceId, status);
}

MeasurementStatus DeviceStateManager::getMeasurementStatus(const QString& deviceId) const {
    QMutexLocker locker(&m_mutex);
    return m_measurementStatuses.value(deviceId);
}



