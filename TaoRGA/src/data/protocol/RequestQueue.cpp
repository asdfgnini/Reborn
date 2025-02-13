#include "RequestQueue.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QMetaObject>

RequestQueue& RequestQueue::getInstance() {
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    static RequestQueue instance;
    return instance;
}

RequestQueue::RequestQueue(QObject *parent) 
    : QObject(parent)
    , m_isProcessing(false)
    , m_networkManager(new QNetworkAccessManager(this)) 
{
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &RequestQueue::handleNetworkReply);
}

RequestQueue::~RequestQueue() {
    clear();
}

void RequestQueue::start() {
    QMutexLocker locker(&m_mutex);
    if (m_requests.isEmpty() || m_isProcessing) {
        return;
    }
    m_isProcessing = true;
    QTimer::singleShot(0, this, &RequestQueue::processNextRequest);
}

void RequestQueue::clear() {
    QMutexLocker locker(&m_mutex);
    m_requests.clear();
    m_isProcessing = false;
}

int RequestQueue::remainingRequests() const {
    QMutexLocker locker(&m_mutex);
    return m_requests.size();
}

void RequestQueue::executeCallback(QThread* targetThread, const std::function<void()>& callback) {
    if (targetThread && QThread::currentThread() != targetThread) {
        QMetaObject::invokeMethod(this, callback, Qt::QueuedConnection);
    } else {
        callback();
    }
}

void RequestQueue::processNextRequest() {
    Request currentRequest;
    {
        QMutexLocker locker(&m_mutex);
        if (m_requests.isEmpty()) {
            m_isProcessing = false;
            qDebug() << "所有请求处理完成";
            return;
        }
        currentRequest = m_requests.takeFirst();
    }

    QUrl url(currentRequest.url);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, currentRequest.contentType);

    QNetworkReply* reply = nullptr;
    if (currentRequest.method.toUpper() == "GET") {
        if (!currentRequest.params.isEmpty()) {
            QUrlQuery query;
            for (auto it = currentRequest.params.constBegin(); it != currentRequest.params.constEnd(); ++it) {
                query.addQueryItem(it.key(), it.value().toString());
            }
            url.setQuery(query);
        }
        reply = m_networkManager->get(request);
    } else if (currentRequest.method.toUpper() == "POST") {
        QJsonDocument doc = QJsonDocument::fromVariant(currentRequest.params);
        reply = m_networkManager->post(request, doc.toJson());
    }

    if (reply) {
        reply->setProperty("successCallback", QVariant::fromValue(currentRequest.successCallback));
        reply->setProperty("failCallback", QVariant::fromValue(currentRequest.failCallback));
        reply->setProperty("targetThread", QVariant::fromValue(currentRequest.targetThread));
    }
}

void RequestQueue::handleNetworkReply(QNetworkReply* reply) {
    reply->deleteLater();

    auto successCallback = reply->property("successCallback").value<std::function<void(const QVariant&)>>();
    auto failCallback = reply->property("failCallback").value<std::function<void(const QString&, int)>>();
    auto targetThread = reply->property("targetThread").value<QThread*>();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        if (successCallback) {
            executeCallback(targetThread, [successCallback, data]() {
                successCallback(QString::fromUtf8(data));
            });
        }
    } else {
        if (failCallback) {
            QString errorString = reply->errorString();
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            executeCallback(targetThread, [failCallback, errorString, statusCode]() {
                failCallback(errorString, statusCode);
            });
        }
    }

    QTimer::singleShot(0, this, &RequestQueue::processNextRequest);
}
