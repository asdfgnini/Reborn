#pragma once

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <qtmetamacros.h>
#include "RingBuffer.h"  // 假设你已经定义了 RingBuffer 类
#include "DataFetcher.h"  // 假设你已经定义了 DataFetcher 类
#include "UrlGenerator.h"



class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onTimeout();
    void onDataUpdated();

private:
    RingBuffer *m_ringBuffer;  // 环形缓冲区
    DataFetcher *m_dataFetcher;  // 数据获取线程
    QTimer m_timer;  // 定时器，用于定时检查缓冲区
    UrlGenerator url1;  // 用于生成 URL 的对象
};
