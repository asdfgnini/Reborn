#include "../include/CustomUI.h"
#include <QVBoxLayout>
#include <QDebug>

CustomUI::CustomUI(QWidget *parent) :
    QWidget(parent),
    m_rgaData(new RGAData(this)),  // 创建 RGAData 对象
    m_isReading(false)  // 初始化标志位
{
    // 设置窗口标题
    setWindowTitle("Custom UI");

    // 创建 UI 控件
    startStopButton = new QPushButton("Start", this);
    amuStartLabel = new QLabel("AMU Start:", this);
    amuStopLabel = new QLabel("AMU Stop:", this);
    ppamuLabel = new QLabel("Ppamu:", this);
    dwellLabel = new QLabel("Dwell:", this);

    // 创建布局并将控件添加到布局中
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(startStopButton);
    layout->addWidget(amuStartLabel);
    layout->addWidget(amuStopLabel);
    layout->addWidget(ppamuLabel);
    layout->addWidget(dwellLabel);
    setLayout(layout);

    // 连接按钮点击事件
    connect(startStopButton, &QPushButton::clicked, this, &CustomUI::toggleReading);
}

CustomUI::~CustomUI()
{
    if (m_readTimer) {
        m_readTimer->stop();
        delete m_readTimer;
    }
    delete m_rgaData;
}

void CustomUI::toggleReading()
{
    if (m_isReading) {
        // 如果当前正在读取数据，则停止子线程获取和解析数据
        m_rgaData->stopThread();
        //停止从环形缓冲区获取数据，刷新完当前剩余数据到前端
        if (m_readTimer) 
        {
            m_readTimer->stop();  // 停止定时器
        }
        startStopButton->setText("Start");  // 设置按钮文本为“Start”
        m_isReading = false;  // 设置标志位为停止状态
    } else {
        // 如果当前没有读取数据，则启动
        if (!m_readTimer) {
            // 创建定时器
            m_readTimer = new QTimer(this);
            connect(m_readTimer, &QTimer::timeout, this, &CustomUI::readFromBuffer);
        }
        m_rgaData->startThread();
        m_readTimer->start(100);  // 启动定时器，每隔100毫秒调用一次读取函数
        startStopButton->setText("Stop");  // 设置按钮文本为“Stop”
        m_isReading = true;  // 设置标志位为读取状态
    }
}

void CustomUI::readFromBuffer()
{
    // 从缓冲区读取数据
    Scan_Data data;
    if (m_rgaData->readFromBuffer(data)) {
        // 读取成功，更新 UI
        updateUIWithData(data);
    } else {
        // qDebug() << "Buffer is empty!";
    }
}

void CustomUI::updateUIWithData(const Scan_Data& data)
{
    // 更新 UI 显示数据
    amuStartLabel->setText("AMU Start: " + QString::number(data.AMU_Start));
    amuStopLabel->setText("AMU Stop: " + QString::number(data.AMU_Stop));
    ppamuLabel->setText("Ppamu: " + QString::number(data.Ppamu));
    dwellLabel->setText("Dwell: " + QString::number(data.Dwell));

    // 调试输出
    qDebug() << "Updated UI with data:" << data.AMU_Start << data.AMU_Stop;
}
