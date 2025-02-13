#pragma once

#include <QString>
#include <QReadWriteLock>
#include <QMutexLocker>
#include <QHash>
#include <QUuid>
#include <optional>  // 引入 std::optional
#include <QList>      // 引入 QList
#include <QDebug>

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
    bool isRunning;           // 是否可以执行这个请求
    RequestStep step;         // 请求的阶段（1 到 5）
    QString responseFormat;   // 数据格式字段，例如 "JSON", "XML", "HTML" 等

    // 更新内部属性，使用 std::optional 使字段可以选择性更新
    void update(const std::optional<QString>& newUrl, 
                const std::optional<bool>& newIsRunning,
                const std::optional<RequestStep>& newStep,
                const std::optional<QString>& newResponseFormat = std::nullopt) {  // 新的字段添加到函数中
        if (newUrl) {
            url = *newUrl;
        }
        if (newIsRunning) {
            isRunning = *newIsRunning;
        }
        if (newStep) {
            step = *newStep;
        }
        if (newResponseFormat) {
            responseFormat = *newResponseFormat;
        }
    }
};


struct RequestFilter {
    std::optional<bool> isRunning;    // 是否正在运行
    std::optional<RequestStep> step;  // 请求阶段
    std::optional<QString> urlPart;   // URL 的一部分（用于包含判断）

    // 判断请求是否符合当前的筛选条件
    bool matches(const UrlRequest& request) const {
        // 如果条件有值，就进行匹配检查
        if (isRunning.has_value() && request.isRunning != isRunning.value()) {
            return false;
        }
        if (step.has_value() && request.step != step.value()) {
            return false;
        }
        if (urlPart.has_value() && !request.url.contains(urlPart.value())) {
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
    QList<QString> addRequests(const QList<UrlRequest>& requestsList) 
    {
        QWriteLocker locker(&lock);  // 写锁，保证线程安全

        QList<QString> requestIds;

        // 遍历所有请求并逐个添加到哈希表中
        for (const UrlRequest& request : requestsList) {
            // 先检查是否已经存在相同 URL 的请求
            QString requestId = findRequestIdByUrl(request.url);
            if (requestId.isEmpty()) {
                // 如果不存在，生成新的请求ID并存储
                requestId = QUuid::createUuid().toString();
                this->requests[requestId] = request;  // 添加到哈希表中
            }
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
    // 通过 URL 查找请求的 ID
    QString findRequestIdByUrl(const QString& url) 
    {
        for (auto it = requests.begin(); it != requests.end(); ++it) 
        {
            if (it.value().url == url) {
                return it.key();  // 返回找到的请求ID
            }
        }
        return QString();  // 如果没有找到，返回空字符串
    }

    // 增加请求并返回唯一的请求ID
    QString addRequest(const UrlRequest& request) 
    {
        QWriteLocker locker(&lock);  // 写锁

        // 先检查是否已经存在相同 URL 的请求
        QString requestId = findRequestIdByUrl(request.url);
        if (requestId.isEmpty()) {
            // 如果不存在，生成新的请求ID并存储
            requestId = QUuid::createUuid().toString();
            requests[requestId] = request;
        }

        return requestId;  // 返回请求ID
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

QString findRequestIdByUrl(const QString& url) const {
    // 遍历所有请求，查找 URL 匹配的请求
    for (auto it = requests.begin(); it != requests.end(); ++it) {
        if (it.value().url == url) {
            return it.key();  // 返回匹配的请求 ID
        }
    }
    return QString();  // 如果没有找到，返回空字符串
}



    // 根据过滤条件批量更新请求,获取符合条件的url,更新其所拥有的属性
void updateRequestsByFilter(const RequestFilter& filter, 
                             const std::optional<bool>& newIsRunning = std::nullopt, 
                             const std::optional<RequestStep>& newStep = std::nullopt) {
    QWriteLocker locker(&lock);  // 获取写锁
#ifdef QT_DEBUG
    qDebug() << "开始更新请求属性...";
#endif
    QList<UrlRequest> result;

    // 遍历所有请求，找到符合筛选条件的请求
    for (auto it = requests.begin(); it != requests.end(); ++it) {
        const UrlRequest& request = it.value();
#ifdef QT_DEBUG
        // 在匹配前打印请求信息
        qDebug() << "检查 URL:" << request.url
                 << ", IsRunning:" << request.isRunning
                 << ", Step:" << stepToString(request.step);
#endif
        // 调用 matches 函数进行条件判断
        if (filter.matches(request)) {
            result.append(request);
        }
    }
#ifdef QT_DEBUG
    qDebug() << "更新找到符合条件的请求数:" << result.size();
#endif
    // 遍历符合条件的请求并更新其属性
    for (auto& request : result) {
        // 更新 isRunning
        if (newIsRunning.has_value()) {
            request.isRunning = newIsRunning.value();
        }

        // 更新 step
        if (newStep.has_value()) {
            request.step = newStep.value();
        }
#ifdef QT_DEBUG
        // 打印更新后的请求信息
        qDebug() << "正在更新 URL:" << request.url
                 << ", IsRunning:" << request.isRunning
                 << ", Step:" << stepToString(request.step);
    }
#endif
    // 将更新后的请求回写到原始列表 requests
    for (auto& request : result) 
    {
        auto it = findRequestIdByUrl(request.url);  // 假设 URL 唯一
            if(requests.contains(it))
            {
                // 更新哈希表中的请求
                requests[it].update(request.url, request.isRunning, request.step); 
#ifdef QT_DEBUG
                qDebug() << "原文件更新 URL:" << request.url;
#endif
            }

        
    }

    // 打印更新后的请求列表
#ifdef QT_DEBUG
    qDebug() << "更新后的请求列表:";
#endif
    for (auto it = requests.begin(); it != requests.end(); ++it) {
        const UrlRequest& updatedRequest = it.value();
        qDebug() << updatedRequest.url
                 << ", IsRunning:" << updatedRequest.isRunning
                 << ", Step:" << stepToString(updatedRequest.step);
    }
}


    // 更新请求的属性（使用 optional 参数来更新）
    void updateRequest(const QString& requestId, 
                    const std::optional<QString>& newUrl = std::nullopt,
                    const std::optional<bool>& newIsRunning = std::nullopt,
                    const std::optional<RequestStep>& newStep = std::nullopt,
                    const std::optional<QString>& newResponseFormat = std::nullopt) {  // 新的字段添加到函数中
        QWriteLocker locker(&lock);  // 写锁
        if (requests.contains(requestId)) {
            requests[requestId].update(newUrl, newIsRunning, newStep, newResponseFormat);
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
        const UrlRequest& request = it.value();
#ifdef TAO_DEBUG      
        // 在匹配前打印请求信息
        qDebug() << "条件检查 URL:" << request.url
                 << ", IsRunning:" << request.isRunning
                 << ", Step:" << stepToString(request.step);
#endif
        // 调用 matches 函数进行条件判断
        if (filter.matches(request)) {
            result.append(request);
        }
    }
#ifdef QT_DEBUG
    qDebug() << "条件获取符合请求数:" << result.size();
#endif
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
    // 打印所有请求的详细信息
void printAllRequests()  {
    QReadLocker locker(&lock);  // 读锁，允许多个线程并发读取

    // 打印表头
    qDebug() << "-----------------------------";
    qDebug() << "Request ID    | URL                                    | Is Running | Step | Response Format";
    qDebug() << "-----------------------------";

    // 遍历哈希表，打印每个请求的信息
    for (auto it = requests.begin(); it != requests.end(); ++it) {
        const UrlRequest& request = it.value();
        QString stepStr = stepToString(request.step);  // 将 RequestStep 转换为字符串
        qDebug() << it.key().leftJustified(15, ' ') << "|"
                 << request.url.leftJustified(40, ' ') << "|"
                 << QString(request.isRunning ? "Yes" : "No").leftJustified(11, ' ') << "|"
                 << stepStr.leftJustified(5, ' ') << "|"
                 << request.responseFormat.leftJustified(15, ' ');
    }

    // 打印当前请求总数
    qDebug() << "URL缓冲区总数: " << requests.size();
    // 打印结束行
    qDebug() << "-----------------------------";
}




private:
    mutable QHash<QString, UrlRequest> requests;  // 使用请求ID作为键的哈希表
    mutable QReadWriteLock lock;                   // 保护请求哈希表的读写锁
};


// // 创建一个 UrlRequestManager 对象
// UrlRequestManager manager;

// // 创建一些请求
// UrlRequest request1 { "http://example.com", false, RequestStep::Step2 };
// UrlRequest request2 { "http://example.org", false, RequestStep::Step2 };
// UrlRequest request3 { "http://example.net", false, RequestStep::Step2 };
// UrlRequest request4 { "http://example.edu", false, RequestStep::Step2 };
// UrlRequest request5 { "http://example.com/about", false, RequestStep::Step2 };

// // 批量添加请求并获取请求ID
// QList<UrlRequest> requests = { request1, request2, request3, request4, request5 };
// QList<QString> requestIds = manager.addRequests(requests);

// // 打印当前所有请求
// qDebug() << "初始请求列表:";
// manager.printAllRequests();

// // 更新请求1的状态，修改为运行中（isRunning = true）并将其阶段更改为 Step4
// manager.updateRequest(requestIds[0], std::nullopt, std::make_optional(true), std::make_optional(RequestStep::Step4));
// qDebug() << "更新请求1的状态，修改为运行中（isRunning = true）并将其阶段更改为 Step4:";
// manager.printAllRequests();
// // 使用过滤器获取符合条件的请求：只获取正在运行（isRunning = true）并且阶段为 Step4 的请求
// RequestFilter filter;
// filter.isRunning = std::make_optional(false);
// filter.step = std::make_optional(RequestStep::Step2);

// // 获取并打印符合条件的请求
// QList<UrlRequest> filteredRequests = manager.getRequestsByFilter(filter);
// qDebug() << "符合条件的请求（isRunning = true, Step = Step4）:";
// for (const UrlRequest& req : filteredRequests) {
//     qDebug() << req.url << req.isRunning << manager.stepToString(req.step);
// }

// // 删除请求：删除 requestId 为 request3 的请求
// manager.removeRequest(requestIds[2]);

// // 打印当前所有请求
// qDebug() << "删除请求后的列表requestIds[2]:";
// manager.printAllRequests();

// // 更新所有符合条件的请求（例如修改阶段为 Step2）
// RequestFilter updateFilter;
// updateFilter.step = std::make_optional(RequestStep::Step2);
// // updateFilter.isRunning = std::make_optional(true);
// manager.updateRequestsByFilter(updateFilter, std::nullopt, std::make_optional(RequestStep::Step5));

// // 打印更新后的所有请求
// qDebug() << "批量更新后的请求列表:";
// manager.printAllRequests();

// // 查找并打印特定请求
// UrlRequest foundRequest = manager.findRequest(requestIds[1]);
// qDebug() << "根据请求ID查找到的请求:" << foundRequest.url << foundRequest.isRunning << manager.stepToString(foundRequest.step);

// // 查找多个请求
// QList<UrlRequest> foundRequests = manager.findRequestsByIds({ requestIds[0], requestIds[4] });
// qDebug() << "根据多个请求ID查找到的请求:";
// for (const UrlRequest& req : foundRequests) {
//     qDebug() << req.url << req.isRunning << manager.stepToString(req.step);
// }

// // 删除所有请求
// manager.removeAllRequests();
// qDebug() << "所有请求已删除。";
// manager.printAllRequests();