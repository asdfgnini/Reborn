#pragma once

#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QHash>
#include <QSharedPointer>
#include <QUuid>
#include <optional>  // 引入 std::optional





//描述URL结构
struct UrlRequest {
    QString url;
    bool isRunning;        // 是否可以执行这个请求
    bool isPeriodic;       // 是否为周期性请求

    // 更新内部属性，使用 std::optional 使字段可以选择性更新
    void update(const std::optional<QString>& newUrl, 
                const std::optional<bool>& newIsRunning, 
                const std::optional<bool>& newIsPeriodic) {
        if (newUrl) {
            url = *newUrl;
        }
        if (newIsRunning) {
            isRunning = *newIsRunning;
        }
        if (newIsPeriodic) {
            isPeriodic = *newIsPeriodic;
        }
    }
};

struct RequestFilter {
    std::optional<bool> isRunning;    // 是否正在运行
    std::optional<bool> isPeriodic;   // 是否为周期性请求

    // 判断请求是否符合当前的筛选条件
    bool matches(const UrlRequest& request) const {
        // 如果条件有值，就进行匹配检查
        if (isRunning.has_value() && request.isRunning != isRunning.value()) {
            return false;
        }
        if (isPeriodic.has_value() && request.isPeriodic != isPeriodic.value()) {
            return false;
        }
        return true;
    }
};


class UrlRequestManager {
public:
    UrlRequestManager() = default;
    ~UrlRequestManager() = default;

    // 批量添加请求并返回请求ID列表
    QList<QString> addRequests(const QList<UrlRequest>& requestsList) {
        QMutexLocker locker(&mutex);  // 加锁，保证线程安全

        QList<QString> requestIds;

        // 遍历所有请求并逐个添加到哈希表中
        for (const UrlRequest& request : requestsList) {
            // 生成唯一的请求ID
            QString requestId = QUuid::createUuid().toString();
            this->requests[requestId] = request;  // 添加到哈希表中
            requestIds.append(requestId);  // 将请求ID添加到返回列表中
        }

        return requestIds;  // 返回请求ID列表
    }


    // 增加请求并返回唯一的请求ID
    QString addRequest(const UrlRequest& request) {
        QMutexLocker locker(&mutex);

        // 生成一个唯一的请求ID（可以是 UUID）
        QString requestId = QUuid::createUuid().toString();

        // 在哈希表中插入请求，使用生成的 requestId 作为键
        requests[requestId] = request;

        // 返回唯一的请求ID
        return requestId;
    }

    // 删除请求
    void removeRequest(const QString& requestId) {
        QMutexLocker locker(&mutex);
        requests.remove(requestId);  // 使用请求ID删除对应的请求
    }

    // 查找请求
    UrlRequest* findRequest(const QString& requestId) {
        QMutexLocker locker(&mutex);
        return requests.contains(requestId) ? &requests[requestId] : nullptr;
    }

    // 更新请求的属性（使用 optional 参数来更新）
    void updateRequest(const QString& requestId, 
                       const std::optional<QString>& newUrl = std::nullopt,
                       const std::optional<bool>& newIsRunning = std::nullopt,
                       const std::optional<bool>& newIsPeriodic = std::nullopt) {
        QMutexLocker locker(&mutex);
        if (requests.contains(requestId)) {
            requests[requestId].update(newUrl, newIsRunning, newIsPeriodic);
        }
    }

    // 获取所有请求（只读访问）
    QList<UrlRequest> getAllRequests() const {
        QMutexLocker locker(&mutex);
        return requests.values();  // 返回所有请求的列表
    }

    // 获取符合条件的请求列表
    QList<UrlRequest> getRequestsByFilter(const RequestFilter& filter) {
        QMutexLocker locker(&mutex);
        QList<UrlRequest> result;
        for (auto it = requests.begin(); it != requests.end(); ++it) {
            if (filter.matches(it.value())) {
                result.append(it.value());
            }
        }
        return result;
    }

private:
    QHash<QString, UrlRequest> requests;  // 使用请求ID作为键的哈希表
    mutable QMutex mutex;                 // 保护请求哈希表的互斥锁
};
