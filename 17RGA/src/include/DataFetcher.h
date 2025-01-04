#pragma once
#include <QThread>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include "HttpClient.h"  // 假设你已经有一个 HttpClient 类
#include "RingBuffer.h"



class DataFetcher : public QThread {
    Q_OBJECT

public:
    DataFetcher(RingBuffer *buffer, const QString &url)
        : m_buffer(buffer), m_url(url) {}

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

        exec();
    }

private:
    RingBuffer *m_buffer;
    QString m_url;

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
