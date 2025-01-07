#include "../include/Mainwindow.h"



MainWindow::MainWindow(QWidget *parent) 
    : QWidget(parent), 
      m_ringBuffer(new RingBuffer(10)),  // 创建大小为 10 的环形缓冲区
      m_dataFetcher(new DataFetcher(m_ringBuffer,&url1, "https://jsonplaceholder.typicode.com/posts?_limit=3")),
      url1("192.168.1.100", 80)  // 初始化 UrlGenerator 对象
{

    connect(&url1, &UrlGenerator::dataUpdated, this, &MainWindow::onDataUpdated);
    url1.startUrlGenerationInThread();    


    // 连接定时器的超时信号与读取缓冲区的槽
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::onTimeout);
    m_timer.start(1000);  // 每秒钟检查一次缓冲区

    // // 启动子线程进行数据获取
    // m_dataFetcher->start();


}

void MainWindow::onTimeout() {
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

void MainWindow::onDataUpdated() {
    // QList<QString> updatedRequestIds = url1.getAllRequestIds();
    // // 处理更新后的请求ID
    // for(const QString& request : updatedRequestIds) {
    //     qDebug() << "found Request ID: " << request;
    // }

    // QList<UrlRequest> foundRequest = url1.findRequestsByIds(updatedRequestIds);

    // for(const UrlRequest& request1 : foundRequest) {
    //     qDebug() << "request1 url: " << request1.url 
    //              << "\n" << "request1 isrunning: " << request1.isRunning  
    //              << "\n" << "request1 step: " << url1.stepToString(request1.step);
    // }

    m_dataFetcher->start();
}
