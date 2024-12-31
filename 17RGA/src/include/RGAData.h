#ifndef RGADATA_H
#define RGADATA_H

#include <QWidget>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QVector>
#include "../include/CircularBuffer.h"

// Scan_Data 结构体定义
struct Scan_Data {
    int AMU_Start;
    int AMU_Stop;
    float Ppamu;
    int Dwell;
    bool Scan_Switch;
    int Current_Scan_Number;
    float Current_TotalPressure;
    float Current_Baseline;
    float Current_TPQuad;
    int Scan_size;
    int Start_Point;
    int Cur_Data_Size;
    int Data_Load_Heavy;
    QList<double> Scan_data_List;
};



class RGAData : public QObject
{
    Q_OBJECT

public:
    explicit RGAData(QObject *parent = nullptr);
    ~RGAData();

    void startThread();  // 开始线程
    void stopThread();   // 停止线程
    bool readFromBuffer(Scan_Data &data);  // 读取缓冲区数据，供消费者调用


private slots:
    void timeOutSlot();  // 定时器超时槽函数
private:
    void fetchData();    // 模拟获取数据
    void parseData();    // 解析数据

private:
    QThread *m_pThread;       // 子线程
    QTimer *m_pTimer;         // 定时器
    Scan_Data m_currentScanData; // 存储扫描数据
    QMutex m_mutex;           // 互斥锁，保护共享资源
    CircularBuffer<Scan_Data> m_buffer; //环形缓冲区 默认大小100
};



#endif // RGADATA_H
