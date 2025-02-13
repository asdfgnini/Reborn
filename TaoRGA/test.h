#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <QMutex>
#include <QMutexLocker>
#include "include/data.h"





class RequestQueue : public QObject {
    Q_OBJECT
public:
    static RequestQueue& getInstance() {
        static RequestQueue instance;
        return instance;
    }

    template<typename T>
    void addRequest(
        const QString &url, 
        const QString &contentType,
        std::function<void(const T&)> parseCallback = nullptr
    ) {
        QMutexLocker locker(&mutex);
        requests.append({url, contentType, parseCallback});
        qDebug() << "添加请求到队列:" << url;
    }


    void start() {
        QMutexLocker locker(&mutex);
        if (requests.isEmpty() || isProcessing) {
            return;
        }
        isProcessing = true;
        // 使用 QTimer 来异步处理下一个请求
        QTimer::singleShot(0, this, &RequestQueue::processNextRequest);
    }

    void clear() {
        QMutexLocker locker(&mutex);
        requests.clear();
        isProcessing = false;
    }

    int remainingRequests() const {
        QMutexLocker locker(&mutex);
        return requests.size();
    }
private:
    template <typename T>
    void fetchDataWithCallback(const QString &url, const QString &contentType, 
                            std::function<void(const QString&)> onSuccess = nullptr,
                            std::function<void(const QString&, int)> onFail = nullptr) 
    {
        HttpClient client(url);

        client.header("Content-Type", contentType)
            .success([url, onSuccess](const QString &response) {
                qDebug() << "Request URL: " << url;

                if constexpr (std::is_same_v<T, QByteArray>) {
                    QByteArray binaryData = response.toUtf8();
                    qDebug() << "Response received, size: " << binaryData.size() << " bytes";
                    qDebug() << "Binary data (hex): " << binaryData.toHex();
                } 
                else if constexpr (std::is_same_v<T, QVariant>) {
                    QVariant jsonData = QVariant::fromValue(response);
                    qDebug() << "Response received as QVariant: " << jsonData.toString();
                }
                
                if (onSuccess) {
                    onSuccess(response);
                }
            })
            .fail([onFail](const QString &error, int statusCode) {
                qDebug() << "Error: " << error << ", Status Code: " << statusCode;
                if (onFail) {
                    onFail(error, statusCode);
                }
            });
            // .debug(true);

        client.get();
    }


private:
    RequestQueue(QObject *parent = nullptr) : QObject(parent) {}
    ~RequestQueue() = default;
    RequestQueue(const RequestQueue&) = delete;
    RequestQueue& operator=(const RequestQueue&) = delete;

    struct Request {
        QString url;
        QString contentType;
        std::function<void(const QVariant&)> parseCallback;
    };
    
    QList<Request> requests;
    bool isProcessing = false;
    mutable QMutex mutex;
    private slots:
    void processNextRequest() {
        Request currentRequest;
        {
            QMutexLocker locker(&mutex);
            if (requests.isEmpty()) {
                isProcessing = false;
                qDebug() << "所有请求处理完成";
                return;
            }
            currentRequest = requests.takeFirst();
        }

        fetchDataWithCallback<QVariant>(
            currentRequest.url, 
            currentRequest.contentType,
            [this, parseCallback = currentRequest.parseCallback](const QString& response) {
                if (parseCallback) {
                    QVariant data = QVariant::fromValue(response);
                    parseCallback(data);
                }
                QTimer::singleShot(0, this, &RequestQueue::processNextRequest);
            },
            [this](const QString& error, int statusCode) {
                QTimer::singleShot(0, this, &RequestQueue::processNextRequest);
            }
        );
    }
};