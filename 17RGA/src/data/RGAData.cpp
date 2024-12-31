#include "../include/RGAData.h"
#include <QDebug>

RGAData::RGAData(QObject *parent)
    : QObject(parent),
      m_pThread(new QThread(this)),
      m_pTimer(new QTimer()),
      m_buffer(100)  // 假设缓冲区大小为 100
{
    // 将定时器移到子线程
    m_pTimer->moveToThread(m_pThread);
    m_pTimer->setInterval(1000);  // 设置每秒触发一次

    // 连接信号和槽
    connect(m_pThread, SIGNAL(started()), m_pTimer, SLOT(start()));
    connect(m_pTimer, &QTimer::timeout, this, &RGAData::timeOutSlot, Qt::DirectConnection);

}

void RGAData::startThread()
{
    if (!m_pThread->isRunning()) {
        m_pThread->start();  // 启动线程
    }
}

void RGAData::stopThread()
{

    m_pThread->quit();  // 停止线程
    m_pThread->wait();
}


RGAData::~RGAData()
{
    // 停止线程
    m_pThread->quit();
    m_pThread->wait();
    delete m_pThread;  // 删除线程
}

void RGAData::timeOutSlot()
{
    qDebug() << "timeOutSlot is running in thread:" << QThread::currentThreadId();

    // 获取数据
    fetchData();

    // 解析数据并存入环形缓冲区
    parseData();
}

void RGAData::fetchData()
{
    // 模拟HTTP请求获取数据

    qDebug() << "fetchData is running in thread:" << QThread::currentThreadId();

    // 示例数据填充
    m_currentScanData.AMU_Start = 1;
    m_currentScanData.AMU_Stop = 10;
    m_currentScanData.Ppamu = 0.05;
    m_currentScanData.Dwell = 2;
    m_currentScanData.Scan_Switch = true;
    m_currentScanData.Current_Scan_Number = 1;
    m_currentScanData.Current_TotalPressure = 101.3;
    m_currentScanData.Current_Baseline = 10.0;
    m_currentScanData.Current_TPQuad = 0.9;
    m_currentScanData.Scan_size = 100;
    m_currentScanData.Start_Point = 0;
    m_currentScanData.Cur_Data_Size = 50;
    m_currentScanData.Data_Load_Heavy = 1;
    m_currentScanData.Scan_data_List = QList<double>{0.1, 0.2, 0.3};  // 模拟数据
}

// 在此解析并将数据存入缓冲区
void RGAData::parseData()
{
    qDebug() << "parseData is running in thread:" << QThread::currentThreadId();

    // 打印当前缓冲区状态
    // qDebug() << "Buffer size before writing: " << m_buffer.size();
    // qDebug() << "Buffer full: " << m_buffer.isFull();

    // 将解析后的数据写入缓冲区
    if (!m_buffer.write(m_currentScanData)) {
        qDebug() << "Failed to write data to buffer!";
    } else {
        qDebug() << "Data successfully written to buffer!";
    }
}


bool RGAData::readFromBuffer(Scan_Data& data)
{
    // 从缓冲区读取数据
    QMutexLocker locker(&m_mutex);  // 自动加锁和解锁
    return m_buffer.read(data);
}
