#include "../include/HttpClient.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>

// HttpClient 实现
HttpClient::HttpClient(QObject* parent)
    : QObject(parent),
      m_manager(new QNetworkAccessManager(this)),
      m_mutex(new QMutex())  // 创建互斥锁
{
    // 网络请求管理器初始化
}

void HttpClient::fetchDeviceStatus(const QString& url)
{
    qDebug() << "HttpClient fetchDeviceStatus  is running in thread:" << QThread::currentThreadId();

    // QMutexLocker locker(m_mutex);  // 加锁，确保线程安全
    // QNetworkRequest request(QUrl{url});
    // QNetworkReply* reply = m_manager->get(request);
    // connect(reply, &QNetworkReply::finished, this, &HttpClient::onStatusFetched);
}

void HttpClient::fetchDeviceData(const QString& url)
{
    qDebug() << "HttpClient fetchDeviceData  is running in thread:" << QThread::currentThreadId();

    // QMutexLocker locker(m_mutex);  // 加锁，确保线程安全
    // QNetworkRequest request(QUrl{url});
    // QNetworkReply* reply = m_manager->get(request);
    // connect(reply, &QNetworkReply::finished, this, &HttpClient::onDataFetched);
}

void HttpClient::onStatusFetched()
{
    QMutexLocker locker(m_mutex);  // 加锁，确保线程安全
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        // 解析状态数据（假设是字符串类型的 JSON 数据）
        QString data = reply->readAll();
        emit deviceStatusFetched(data);  // 发射状态数据获取完成的信号
    } else {
        qWarning() << "Failed to fetch device status:" << reply->errorString();
    }

    reply->deleteLater();
}

void HttpClient::onDataFetched()
{
    QMutexLocker locker(m_mutex);  // 加锁，确保线程安全
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        // 解析数据（假设是字符串类型的 JSON 数据）
        QString data = reply->readAll();
        emit deviceDataFetched(data);  // 发射数据获取完成的信号
    } else {
        qWarning() << "Failed to fetch device data:" << reply->errorString();
    }

    reply->deleteLater();
}

HttpClient::~HttpClient()
{
    delete m_mutex;  // 销毁互斥锁
}
