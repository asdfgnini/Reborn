#pragma once

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <qtmetamacros.h>
#include "RingBuffer.h"  // 假设你已经定义了 RingBuffer 类
#include "DataFetcher.h"  // 假设你已经定义了 DataFetcher 类
#include "UrlGenerator.h"
#include <QPushButton>  // 引入 QPushButton 头文件

#include "../ui/inc/ui.h"
class MainWindow : public TaoWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTimeout();             // 定时器超时槽函数
    void onDataUpdated();         // 数据更新槽函数
    void onButtonClicked();       // 按钮点击槽函数

private:
    QTimer m_timer;               // 定时器
    RingBuffer *m_ringBuffer;     // 环形缓冲区
    DataFetcher *m_dataFetcher;   // 数据获取器
    UrlGenerator url1;            // URL 生成器
private:
    void ui_init();
    void initWindow();

    void initEdgeLayout();

};

