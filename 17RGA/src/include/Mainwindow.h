#pragma once

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "RingBuffer.h"  // 假设你已经定义了 RingBuffer 类
#include "DataFetcher.h"  // 假设你已经定义了 DataFetcher 类

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) 
        : QWidget(parent), 
          m_ringBuffer(new RingBuffer(10)),  // 创建大小为 10 的环形缓冲区
          m_dataFetcher(new DataFetcher(m_ringBuffer, "https://jsonplaceholder.typicode.com/posts?_limit=3"))
    {
        // 连接定时器的超时信号与读取缓冲区的槽
        connect(&m_timer, &QTimer::timeout, this, &MainWindow::onTimeout);
        m_timer.start(1000);  // 每秒钟检查一次缓冲区

        // 启动子线程进行数据获取
        m_dataFetcher->start();
    }

private slots:
    void onTimeout() {
        qDebug() << "onTimeout is running " << QThread::currentThreadId();

        Post post;
        if (m_ringBuffer->read(post)) { 
            qDebug() << "Post ID:" << post.id;
            qDebug() << "User ID:" << post.userId;
            qDebug() << "Title:" << post.title;
            qDebug() << "Body:" << post.body;
        } else {
            qDebug() << "No new data yet...";
        }
    }

private:
    RingBuffer *m_ringBuffer;  // 环形缓冲区
    DataFetcher *m_dataFetcher;  // 数据获取线程
    QTimer m_timer;  // 定时器，用于定时检查缓冲区
};
