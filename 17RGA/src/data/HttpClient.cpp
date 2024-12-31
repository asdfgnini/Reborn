// HttpClient.cpp
#include "../include/HttpClient.h"
#include <QDebug>
#include <QThread>


HttpClient::HttpClient(QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this)),m_isRequestInProgress(false)
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &HttpClient::onFinished);
}


HttpClient::~HttpClient()
{
    
}

void HttpClient::fetchData(const QString &url)
{
    qDebug() << "HttpClient fetchData is running in thread:" << QThread::currentThreadId();

    if (m_isRequestInProgress) {
        qDebug() << "Previous request is still in progress!";
        return;  // 如果上一个请求还没有完成，则不发起新请求
    }

    m_isRequestInProgress = true;
    qDebug() << "HttpClient Fetching data from URL: " << url;

    QUrl qUrl(url);
    QNetworkRequest request(qUrl);

    // 发送GET请求
    m_reply = m_manager->get(request);

    // 检查 QNetworkReply 是否有效
    if (!m_reply) 
    {
        qDebug() << "Error: QNetworkReply object is nullptr!";
        m_isRequestInProgress = false;  // 请求失败，重置状态
        return;
    }

    // 连接 finished 信号
    connect(m_reply, &QNetworkReply::finished, this, &HttpClient::onFinished);

    // 设置超时定时器
}

#include <QFile>
#include <QTextStream>

void HttpClient::onFinished()
{
    qDebug() << "HttpClient onFinished is running in thread:" << QThread::currentThreadId();

    // 获取 reply 对象
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    // 确保 reply 非空
    if (!reply) {
        qDebug() << "Error: reply is null!";
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error fetching data:" << reply->errorString();
    } else {
        qDebug() << "Response received from URL:" << reply->url().toString();

        // 获取响应的字节数组
        QByteArray responseData = reply->readAll();
        qDebug() << "Raw response data:" << responseData;

        // 将响应数据保存到文件
        saveJsonToFile(responseData);

        // 从字节数组解析 JSON 数据
        QJsonDocument doc = QJsonDocument::fromJson(responseData);

        if (doc.isObject()) {
            // 如果是 JSON 对象
            QJsonObject jsonObj = doc.object();

            // 将解析后的 JSON 转换为 QVariant 进行更好的处理
            QVariantMap dataMap = jsonObj.toVariantMap();

            // 打印解析后的 JSON 数据
            qDebug() << "Parsed JSON object:";
            // printJsonData(dataMap); // 递归打印数据结构
        } else {
            qDebug() << "Invalid JSON format!";
        }
    }

    m_isRequestInProgress = false;  // 请求完成，重置状态

    // 删除 reply
    reply->deleteLater();
}

// 将 JSON 数据写入文件
void HttpClient::saveJsonToFile(const QByteArray &data)
{
    // 打开或创建文件，写入响应的 JSON 数据
    QFile file("response_data.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << data;  // 将 JSON 数据写入文件
        file.close();  // 关闭文件
        qDebug() << "JSON data saved to response_data.json";
    } else {
        qDebug() << "Error opening file for writing!";
    }
}




