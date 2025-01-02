#include "../include/RGAData.h"
#include <QDebug>

RGAData::RGAData(QObject *parent) 
    : QObject(parent),
      m_client(new HttpClient(this)),
      m_timerManager(new MultiMY_Timer(this))
{
    // 连接信号和槽
    connect(m_client, &HttpClient::deviceStatusFetched, this, &RGAData::onDeviceStatusFetched);
    connect(m_client, &HttpClient::deviceDataFetched, this, &RGAData::onDeviceDataFetched);
    
    // 添加定时器周期性获取设备状态和数据
    m_timerManager->addTimer(1, this, &RGAData::fetchDeviceStatus, 1000,true);  // 每10秒获取一次设备状态
    m_timerManager->addTimer(2, this, &RGAData::fetchDeviceData, 1000,true);   // 每5秒获取一次设备数据
}

void RGAData::fetchDeviceStatus()
{
    qDebug() << "fetchDeviceStatus  is running in thread:" << QThread::currentThreadId();

    m_client->fetchDeviceStatus("http://device-status-url");
}

void RGAData::fetchDeviceData()
{
    qDebug() << "fetchDeviceData  is running in thread:" << QThread::currentThreadId();


    m_client->fetchDeviceData("http://device-data-url");
}

void RGAData::onDeviceStatusFetched(const QString &data)
{
    // 处理设备状态数据
    qDebug() << "Device status fetched:" << data;
}

void RGAData::onDeviceDataFetched(const QString &data)
{
    // 处理设备数据
    qDebug() << "Device data fetched:" << data;
}

void RGAData::start()
{
    // 启动定时器
    m_timerManager->startTimer(1);
    m_timerManager->startTimer(2);
}
