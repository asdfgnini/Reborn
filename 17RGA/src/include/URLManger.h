#pragma once

#include <QString>
#include <QReadWriteLock>
#include <QMutexLocker>
#include <QHash>
#include <QUuid>
#include <optional>  // 引入 std::optional
#include <QList>      // 引入 QList

// 请求的阶段（1 到 5）
enum class RequestStep {
    Invalid = -1,  // 添加无效值，通常我们用 -1 来表示无效或默认状态
    Step1 = 1,  // 阶段 1
    Step2 = 2,  // 阶段 2
    Step3 = 3,  // 阶段 3
    Step4 = 4,  // 阶段 4
    Step5 = 5   // 阶段 5
};

// 描述 URL 结构
struct UrlRequest {
    QString url;
    bool isRunning;        // 是否可以执行这个请求
    RequestStep step;      // 请求的阶段（1 到 5）

    // 更新内部属性，使用 std::optional 使字段可以选择性更新
    void update(const std::optional<QString>& newUrl, 
                const std::optional<bool>& newIsRunning,
                const std::optional<RequestStep>& newStep) {
        if (newUrl) {
            url = *newUrl;
        }
        if (newIsRunning) {
            isRunning = *newIsRunning;
        }
        if (newStep) {
            step = *newStep;
        }
    }
};

struct RequestFilter {
    std::optional<bool> isRunning;    // 是否正在运行
    std::optional<RequestStep> step;  // 请求阶段

    // 判断请求是否符合当前的筛选条件
    bool matches(const UrlRequest& request) const {
        // 如果条件有值，就进行匹配检查
        if (isRunning.has_value() && request.isRunning != isRunning.value()) {
            return false;
        }
        if (step.has_value() && request.step != step.value()) {
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
        QWriteLocker locker(&lock);  // 写锁，保证线程安全

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

    // 为枚举实现一个 toString 函数
    QString stepToString(RequestStep step) {
        switch (step) {
            case RequestStep::Step1: return "Step1";
            case RequestStep::Step2: return "Step2";
            case RequestStep::Step3: return "Step3";
            case RequestStep::Step4: return "Step4";
            case RequestStep::Step5: return "Step5";
            default: return "Unknown";
        }
    }

    // 字符串到枚举转换
    RequestStep stringToStep(const QString& stepString) {
        if (stepString == "Step1") return RequestStep::Step1;
        if (stepString == "Step2") return RequestStep::Step2;
        if (stepString == "Step3") return RequestStep::Step3;
        if (stepString == "Step4") return RequestStep::Step4;
        if (stepString == "Step5") return RequestStep::Step5;
        return RequestStep::Step1;  // 默认返回 Step1，或者可以选择返回一个无效的枚举值
    }

    // 增加请求并返回唯一的请求ID
    QString addRequest(const UrlRequest& request) {
        QWriteLocker locker(&lock);  // 写锁

        // 生成一个唯一的请求ID（可以是 UUID）
        QString requestId = QUuid::createUuid().toString();

        // 在哈希表中插入请求，使用生成的 requestId 作为键
        requests[requestId] = request;

        // 返回唯一的请求ID
        return requestId;
    }

    // 删除请求
    void removeRequest(const QString& requestId) {
        QWriteLocker locker(&lock);  // 写锁
        requests.remove(requestId);  // 使用请求ID删除对应的请求
    }

    // 查找请求
    UrlRequest findRequest(const QString& requestId) {
        QReadLocker locker(&lock);  // 读锁，允许并发读取
        return requests.contains(requestId) ? requests[requestId] : UrlRequest();
    }

    // 更新请求的属性（使用 optional 参数来更新）
    void updateRequest(const QString& requestId, 
                       const std::optional<QString>& newUrl = std::nullopt,
                       const std::optional<bool>& newIsRunning = std::nullopt,
                       const std::optional<RequestStep>& newStep = std::nullopt) {
        QWriteLocker locker(&lock);  // 写锁
        if (requests.contains(requestId)) {
            requests[requestId].update(newUrl, newIsRunning, newStep);
        }
    }

    // 获取所有请求（只读访问）
    QList<UrlRequest> getAllRequests() const {
        QReadLocker locker(&lock);  // 读锁，允许多个线程并发读取
        return requests.values();  // 返回所有请求的列表
    }

    // 获取符合条件的请求列表
    QList<UrlRequest> getRequestsByFilter(const RequestFilter& filter) {
        QReadLocker locker(&lock);  // 读锁
        QList<UrlRequest> result;
        for (auto it = requests.begin(); it != requests.end(); ++it) {
            if (filter.matches(it.value())) {
                result.append(it.value());
            }
        }
        return result;
    }

    // 批量查找请求，根据请求ID列表
    QList<UrlRequest> findRequestsByIds(const QList<QString>& requestIds) {
        QReadLocker locker(&lock);  // 读锁
        QList<UrlRequest> foundRequests;

        // 遍历传入的请求 ID 列表，查找对应的请求
        for (const QString& requestId : requestIds) {
            if (requests.contains(requestId)) {
                foundRequests.append(requests[requestId]);
            }
        }

        return foundRequests;  // 返回找到的请求列表
    }

    // 删除所有请求
    void removeAllRequests() {
        QWriteLocker locker(&lock);  // 写锁
        requests.clear();  // 清空所有请求
    }

private:
    mutable QHash<QString, UrlRequest> requests;  // 使用请求ID作为键的哈希表
    mutable QReadWriteLock lock;                   // 保护请求哈希表的读写锁
};
