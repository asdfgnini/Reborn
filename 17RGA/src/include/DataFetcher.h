#pragma once
#include <QThread>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "HttpClient.h"  // 假设你已经有一个 HttpClient 类
#include "RingBuffer.h"
#include "./UrlGenerator.h"


class DataFetcher : public QThread {
    Q_OBJECT

public:
    DataFetcher(RingBuffer *buffer,UrlGenerator* urlGenerator, const QString &url)
        : m_buffer(buffer), m_url(url),m_urlGenerator(urlGenerator) 
        {
            isConnect = true;
            isScan = false;
            currentStep = RequestStep::Step1;
        }
        void startFetching() 
        {
            // 设置线程为活动状态
            exitCondition = false;
            start();  // 启动线程，自动调用 run()
        }

        void stopFetching() 
        {
            // 设置退出条件，停止线程
            exitCondition = true;
            quit();  // 退出事件循环
            wait();  // 等待线程退出
        }

        ~DataFetcher() {
            stopFetching();  // 确保在销毁对象时线程已停止
        }

private:
void processStep1()
{
    // 如果上一个请求还没有完成，则跳过当前请求
    if (isRequestInProgress) {
        return;
    }

    qDebug() << "processStep1 is running " << QThread::currentThreadId();

    QList<QString> updatedRequestIds = m_urlGenerator->getAllRequestIds();
    // 处理更新后的请求ID
    for(const QString& request : updatedRequestIds) {
        qDebug() << "found Request ID: " << request;
    }

    QList<UrlRequest> urlRequests = m_urlGenerator->findRequestsByIds(updatedRequestIds);

    for(const UrlRequest& request1 : urlRequests) {
        qDebug() << "request1 url: " << request1.url 
                << "\n" << "request1 isrunning: " << request1.isRunning  
                << "\n" << "request1 step: " << m_urlGenerator->stepToString(request1.step);
    }

    if (urlRequests.isEmpty()) {
        qDebug() << "No URL requests available in the buffer.";
        return;
    }

    // 标记当前请求正在进行中
    isRequestInProgress = true;

    // 处理每个 UrlRequest
    for (const UrlRequest& request : urlRequests) {
        qDebug() << "Processing request with URL: " << request.url;

        // 创建 HTTP 客户端
        HttpClient client(request.url);  // 使用 UrlRequest 中的 URL

        // 设置请求头等信息
        client.header("Content-Type", "application/json")
              .success([this, request](const QString &response) {
                  qDebug() << "Request succeeded for URL: " << request.url;
                  qDebug() << "Response received: " << response;

                  // 解析 JSON 响应
                  QList<Post> posts = parseJsonResponse(response);

              })
              .fail([this, request](const QString &error, int statusCode) {
                  qDebug() << "Request failed for URL: " << request.url
                           << ". Error: " << error << ", Status Code: " << statusCode;
              })
              .complete([this]() {
                  qDebug() << "Request completed.";
                  isRequestInProgress = false;

                    exitCondition = true;
                  // 根据需要决定是否退出或继续处理
                  if (exitCondition) {
                      quit();  // 退出事件循环
                  }
              })
              .debug(true);  // 开启调试模式，打印请求 URL 和参数等信息

        client.get();  // 发送 GET 请求
    }
}

void processStep2()
{

}
void processStep3()
{

}
void processStep4()
{

}
void processStep5()
{

}

protected:
    void run() 
    {
        qDebug() << "DataFetch is running " << QThread::currentThreadId();

        // 启动事件循环
        while (!exitCondition) {
            switch (currentStep) {
            case RequestStep::Step1:
                if (isConnect) {
                    processStep1();  // 处理第一阶段的 URL
                }
                break;
            case RequestStep::Step2:
                if (isConnect) {
                    processStep2();  // 处理第二阶段的 URL
                }
                break;
            case RequestStep::Step3:
                if (isScan) {
                    processStep3();  // 处理第三阶段的 URL
                }
                break;
            case RequestStep::Step4:
                processStep4();  // 处理第四阶段的 URL
                break;
            case RequestStep::Step5:
                processStep5();  // 处理第五阶段的 URL
                break;
            default:
                // 空转，等待状态变化
                QThread::msleep(100);  // 等待状态变化
                break;
            }

            // 启动事件循环并处理事件
            QCoreApplication::processEvents();  // 处理等待的事件
        }
    }


private:
    RingBuffer *m_buffer;
    QString m_url;
    UrlGenerator* m_urlGenerator;
    bool exitCondition = false;  // 控制线程退出的条件
    // 当前状态
    bool isConnect;  // 是否进行连接
    bool isScan;     // 是否进行扫描
    RequestStep currentStep;  // 当前请求阶段
    bool isRequestInProgress = false;  // 请求进行中的标志


QList<Post> parseJsonResponse(const QString &response)
{
    QList<Post> posts;

    // 打印原始的 JSON 响应数据
    qDebug() << "Raw JSON response: " << response;

    // 将字符串转换为 QJsonDocument
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());

    // 检查 JSON 是否有效
    if (!doc.isNull()) {
        qDebug() << "JSON is valid.";

        // 如果是一个 JSON 数组
        if (doc.isArray()) {
            QJsonArray jsonArray = doc.array();
            qDebug() << "Parsing JSON array, number of elements: " << jsonArray.size();

            // 遍历数组中的每个元素
            for (const QJsonValue &value : jsonArray) {
                QJsonObject jsonObject = value.toObject();
                Post post;

                // 解析每个字段并打印
                post.data = jsonObject["data"].toString();
                post.name = jsonObject["name"].toString();
                post.origin = jsonObject["origin"].toString();

                qDebug() << ", Data: " << post.data
                         << ", Name: " << post.name
                         << ", Origin: " << post.origin;

                posts.append(post);
            }
        } else {
            qDebug() << "Expected a JSON array, but the response is not an array.";
        }
    } else {
        qDebug() << "Invalid JSON response.";
    }

    return posts;
}

};
