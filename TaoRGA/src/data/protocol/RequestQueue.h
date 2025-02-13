#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QString>
#include <QUrl>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include <functional>
#include <QDebug>

class RequestQueue : public QObject {
    Q_OBJECT
public:
    static RequestQueue& getInstance();

    template<typename T>
    void addRequest(
        const QString &url, 
        const QString &method = "GET",
        const QVariantMap &params = QVariantMap(),
        const QString &contentType = "application/json",
        std::function<void(const T&)> successCallback = nullptr,
        std::function<void(const QString&, int)> failCallback = nullptr,
        QThread* targetThread = nullptr
    ) {
        QMutexLocker locker(&m_mutex);
        if (!targetThread) {
            targetThread = QThread::currentThread();
        }

        Request request;
        request.url = url;
        request.method = method;
        request.params = params;
        request.contentType = contentType;
        request.successCallback = [successCallback](const QVariant& response) {
            if (successCallback) {
                if constexpr (std::is_same_v<T, QByteArray>) {
                    successCallback(response.toByteArray());
                } else if constexpr (std::is_same_v<T, QString>) {
                    successCallback(response.toString());
                } else if constexpr (std::is_same_v<T, QJsonObject>) {
                    QJsonDocument doc = QJsonDocument::fromJson(response.toString().toUtf8());
                    successCallback(doc.object());
                } else {
                    successCallback(response);
                }
            }
        };
        request.failCallback = failCallback;
        request.targetThread = targetThread;

        m_requests.append(request);
        qDebug() << "添加请求到队列:" << url << "目标线程:" << targetThread;
        
        if (!m_isProcessing) {
            start();
        }
    }

    void start();
    void clear();
    int remainingRequests() const;

private:
    struct Request {
        QString url;
        QString method;
        QVariantMap params;
        QString contentType;
        std::function<void(const QVariant&)> successCallback;
        std::function<void(const QString&, int)> failCallback;
        QThread* targetThread;
    };

    explicit RequestQueue(QObject *parent = nullptr);
    ~RequestQueue();
    RequestQueue(const RequestQueue&) = delete;
    RequestQueue& operator=(const RequestQueue&) = delete;

    void executeCallback(QThread* targetThread, const std::function<void()>& callback);
    
    QList<Request> m_requests;
    bool m_isProcessing;
    mutable QMutex m_mutex;
    QNetworkAccessManager* m_networkManager;

private slots:
    void processNextRequest();
    void handleNetworkReply(QNetworkReply* reply);
};
