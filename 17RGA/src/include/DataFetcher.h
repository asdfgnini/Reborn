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
        : m_buffer(buffer), m_url(url),m_urlGenerator(urlGenerator) {}

protected:
    void run() override {

        HttpClient client(m_url);
        client.header("Content-Type", "application/json")
              .success([this](const QString &response) {
                qDebug() << "success is running " << QThread::currentThreadId();

                  QList<Post> posts = parseJsonResponse(response);
                  for (const Post &post : posts) {
                      m_buffer->write(post);
                  }
              })
              .fail([](const QString &error, int statusCode) {
                  qDebug() << "Request failed. Error:" << error << ", Status Code:" << statusCode;
              })
              .complete([]() {
                qDebug() << "complete is running " << QThread::currentThreadId();

                  qDebug() << "Request completed.";
              })
              .debug(true); // 开启调试模式，打印请求 URL 和参数等信息

            client.get(); // 发送 GET 请求

            QList<QString> updatedRequestIds = m_urlGenerator->getAllRequestIds();
            // 处理更新后的请求ID
            for(const QString& request : updatedRequestIds) {
                qDebug() << "found Request ID: " << request;
            }

            QList<UrlRequest> foundRequest = m_urlGenerator->findRequestsByIds(updatedRequestIds);

            for(const UrlRequest& request1 : foundRequest) {
                qDebug() << "request1 url: " << request1.url 
                        << "\n" << "request1 isrunning: " << request1.isRunning  
                        << "\n" << "request1 step: " << m_urlGenerator->stepToString(request1.step);
            }
        exec();
    }

private:
    RingBuffer *m_buffer;
    QString m_url;
    UrlGenerator* m_urlGenerator;

    QList<Post> parseJsonResponse(const QString &response) {
    qDebug() << "parseJsonResponse is running " << QThread::currentThreadId();

        QList<Post> posts;
        QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
        if (doc.isArray()) {
            QJsonArray array = doc.array();
            for (const QJsonValue &value : array) {
                QJsonObject obj = value.toObject();
                Post post;
                post.userId = obj["userId"].toInt();
                post.id = obj["id"].toInt();
                post.title = obj["title"].toString();
                post.body = obj["body"].toString();
                posts.append(post);
            }
        }
        return posts;
    }
};
