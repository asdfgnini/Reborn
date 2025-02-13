#pragma once

#include "DeviceStateManager.h"
#include <QString>
#include <QReadWriteLock>
#include <QMutexLocker>
#include <QHash>
#include <QUuid>
#include <optional>  // 引入 std::optional
#include <QList>      // 引入 QList
#include <QDebug>
#include <QVariant>
#include <QJsonObject>

// 请求的阶段（1 到 5）, 主要用于应付外界的事件，然后改变自身属性
enum class RequestStep {
    Invalid = -1,  // 添加无效值，通常我们用 -1 来表示无效或默认状态
    Step1 = 1,  // 阶段 1
    Step2 = 2,  // 阶段 2
    Step3 = 3,  // 阶段 3
    Step4 = 4,  // 阶段 4
    Step5 = 5,  // 阶段 5
    Step6 = 6,  // 阶段 6
    Step7 = 7,  // 阶段 7
    Step8 = 8,  // 阶段 8
    Step9 = 9,  // 阶段 9
    Step10 = 10,  // 阶段 10
};


enum class UrlMatchType {
    Contains,           // 包含关系
    StartsWith,        // 以特定字符串开始
    EndsWith,          // 以特定字符串结束
    ExactMatch         // 精确匹配
};

enum class RequestType {
    Invalid = -1,
    ScanStatus,
    SensorStatus,
    SystemStatus,
    DeviceBaseInfo,
    ControlInfo,
    Measurement,
};



// 定义解析路径结构
struct ParsePath {
    QString key;           // JSON路径
    QString description;   // 路径描述/标识
    QString dataType;      // 数据类型（如："int", "string", "bool"等）
    QVariant defaultValue; // 默认值
};

// 描述 URL 结构
struct UrlRequest {
    QString url;
    bool isRunning;
    QList<RequestStep> steps;
    QString responseFormat;
    int priority;  // 新增的字段，用于表示优先级
    bool needParse = false;
    QMap<QString, ParsePath> parsePaths;  // 修改为 QMap
    RequestType requestType;

    // // 更新方法，支持优先级字段
    // void update(const std::optional<QString>& newUrl, 
    //             const std::optional<bool>& newIsRunning,
    //             const std::optional<QList<RequestStep>>& newSteps,
    //             const std::optional<QString>& newResponseFormat,
    //             const std::optional<int>& newPriority) 
    // {
    //     if (newUrl) {
    //         url = *newUrl;
    //     }
    //     if (newIsRunning) {
    //         isRunning = *newIsRunning;
    //     }
    //     if (newSteps) {
    //         steps = *newSteps;
    //     }
    //     if (newResponseFormat) {
    //         responseFormat = *newResponseFormat;
    //     }
    //     if (newPriority) {
    //         priority = *newPriority;  // 更新优先级
    //     }
    // }
};



struct RequestFilter {
    std::optional<bool> isRunning;
    std::optional<QList<RequestStep>> steps;
    std::optional<QString> urlPart;
    std::optional<QString> responseFormat;
    std::optional<int> priority;  // 新增的筛选条件

    bool matches(const UrlRequest& request) const {
        if (isRunning.has_value() && request.isRunning != isRunning.value()) {
            return false;
        }
        if (steps.has_value()) {
            bool stepMatches = false;
            for (const RequestStep& step : steps.value()) {
                if (request.steps.contains(step)) {
                    stepMatches = true;
                    break;
                }
            }
            if (!stepMatches) {
                return false;
            }
        }
        if (urlPart.has_value() && !request.url.contains(urlPart.value())) {
            return false;
        }
        if (responseFormat.has_value() && request.responseFormat != responseFormat.value()) {
            return false;
        }
        if (priority.has_value() && request.priority != priority.value()) {  // 根据优先级过滤
            return false;
        }
        return true;
    }
};



class UrlRequestManager {
public:
    UrlRequestManager() = default;
    ~UrlRequestManager() = default;

    // 将字符串转换为 RequestStep
    RequestStep stringToStep(const QString& stepString) {
        if (stepString == "Step1") {
            return RequestStep::Step1;
        } else if (stepString == "Step2") {
            return RequestStep::Step2;
        } else if (stepString == "Step3") {
            return RequestStep::Step3;
        } else if (stepString == "Step4") {
            return RequestStep::Step4;
        } else if (stepString == "Step5") {
            return RequestStep::Step5;
        } else if (stepString == "Step6") {
            return RequestStep::Step6;
        } else if (stepString == "Step7") {
            return RequestStep::Step7;
        } else if (stepString == "Step8") {
            return RequestStep::Step8;
        } else if (stepString == "Step9") {
            return RequestStep::Step9;
        } else if (stepString == "Step10") {
            return RequestStep::Step10;
        } else {
            return RequestStep::Invalid;  // 如果字符串无法匹配任何步骤，返回 Unknown
        }
    }
    // 将 RequestStep 转换为字符串
    QString stepToString(RequestStep step) {
        switch (step) {
            case RequestStep::Step1:
                return "Step1";
            case RequestStep::Step2:
                return "Step2";
            case RequestStep::Step3:
                return "Step3";
            case RequestStep::Step4:
                return "Step4";
            case RequestStep::Step5:
                return "Step5";
            case RequestStep::Step6:
                return "Step6";
            case RequestStep::Step7:
                return "Step7";
            case RequestStep::Step8:
                return "Step8";
            case RequestStep::Step9:
                return "Step9";
            case RequestStep::Step10:
                return "Step10";
            default:
                return "Invalid";  // 如果是未知的枚举值，返回 "Unknown"
        }
    }

    QString requestTypeToString(RequestType requestType) {
        switch (requestType) {
            case RequestType::ScanStatus:
                return "ScanStatus";
            case RequestType::ControlInfo:
                return "ControlInfo";
            case RequestType::SensorStatus:
                return "SensorStatus";
            case RequestType::SystemStatus:
                return "SystemStatus";
            case RequestType::DeviceBaseInfo:
                return "DeviceBaseInfo";
            default:
                return "Unknown";
        }
    }


    RequestType stringToRequestType(const QString& requestTypeString) {
        if (requestTypeString == "ScanStatus") {
            return RequestType::ScanStatus;
        }
        else if (requestTypeString == "ControlInfo") {
            return RequestType::ControlInfo;
        }

        else if (requestTypeString == "SensorStatus") {
            return RequestType::SensorStatus;
        }
        else if (requestTypeString == "SystemStatus") {
            return RequestType::SystemStatus;
        }
        else if (requestTypeString == "DeviceBaseInfo") {
            return RequestType::DeviceBaseInfo;
        }
        return RequestType::Invalid;
    }

// 增加请求并返回唯一的请求ID
    QString addRequest(const UrlRequest& request) {
        QWriteLocker locker(&lock);  // 写锁，确保线程安全

        // 先检查是否已经存在相同 URL 的请求
        QString requestId = findRequestIdByUrl(request.url);
        if (requestId.isEmpty()) {
            // 如果不存在，生成新的请求ID并存储
            requestId = QUuid::createUuid().toString();
            requests[requestId] = request;
        }

        return requestId;  // 返回请求ID
    }

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
    // // 更新请求的属性（支持多个 step）
    // void updateRequest(const QString& requestId, 
    //                 const std::optional<QString>& newUrl = std::nullopt,
    //                 const std::optional<bool>& newIsRunning = std::nullopt,
    //                 const std::optional<QList<RequestStep>>& newSteps = std::nullopt,
    //                 const std::optional<QString>& newResponseFormat = std::nullopt,
    //                 const std::optional<int>& newPriority = std::nullopt) 
    // {
    //     QWriteLocker locker(&lock);

    //     if (requests.contains(requestId)) {
    //         UrlRequest& request = requests[requestId];

    //         // 更新 URL、isRunning 和 responseFormat
    //         request.update(newUrl, newIsRunning, newSteps, newResponseFormat, newPriority);
    //     }
    // }


    // // 根据 UrlRequest 对象更新请求属性（支持多个步骤）
    // void updateRequest(const UrlRequest& request,
    //                 const std::optional<bool>& newIsRunning = std::nullopt,
    //                 const std::optional<QList<RequestStep>>& newSteps = std::nullopt,
    //                 const std::optional<QString>& newResponseFormat = std::nullopt,
    //                 const std::optional<int>& newPriority = std::nullopt)
    // {
    //     QWriteLocker locker(&lock);  // 写锁
    //     QString requestId = findRequestIdByUrl(request.url);  // 通过 URL 查找请求 ID

    //     if (!requestId.isEmpty()) {
    //         requests[requestId].update(request.url, newIsRunning, newSteps, newResponseFormat,newPriority);
    //     } else {
    //         qWarning() << "Request with URL" << request.url << "does not exist!";
    //     }
    // }


    // // 根据过滤条件批量更新请求
    // void updateRequestsByFilter(const RequestFilter& filter, 
    //                              const std::optional<bool>& newIsRunning = std::nullopt, 
    //                              const std::optional<QList<RequestStep>>& newSteps = std::nullopt,
    //                              const std::optional<QString>& newResponseFormat = std::nullopt,
    //                              const std::optional<int>& newPriority = std::nullopt) {
    //     QWriteLocker locker(&lock);  // 获取写锁
    //     QList<UrlRequest> result;

    //     // 遍历所有请求，找到符合筛选条件的请求
    //     for (auto it = requests.begin(); it != requests.end(); ++it) {
    //         const UrlRequest& request = it.value();
    //         if (filter.matches(request)) {
    //             result.append(request);
    //         }
    //     }

    //     // 遍历符合条件的请求并更新其属性
    //     for (auto& request : result) {
    //         if (newIsRunning.has_value()) {
    //             request.isRunning = newIsRunning.value();
    //         }
    //         if (newSteps.has_value()) {
    //             request.steps = newSteps.value();
    //         }
    //         if (newResponseFormat.has_value()) {
    //             request.responseFormat = newResponseFormat.value();
    //         }
    //     }

    //     // 将更新后的请求回写到原始列表 requests
    //     for (auto& request : result) {
    //         auto it = findRequestIdByUrl(request.url);
    //         if (requests.contains(it)) {
    //             requests[it].update(request.url, request.isRunning, request.steps, request.responseFormat,newPriority);
    //         }
    //     }
    // }

    // 获取所有请求（只读访问）
    QList<UrlRequest> getAllRequests() const {
        QReadLocker locker(&lock);  // 读锁，允许多个线程并发读取
        return requests.values();  // 返回所有请求的列表
    }

    // 查找请求
    UrlRequest findRequest(const QString& requestId) {
        QReadLocker locker(&lock);  // 读锁，允许并发读取
        return requests.contains(requestId) ? requests[requestId] : UrlRequest();
    }

    // 根据请求ID列表查找多个请求
    QList<UrlRequest> findRequestsByIds(const QList<QString>& requestIds) {
        QReadLocker locker(&lock);  // 读锁，确保线程安全

        QList<UrlRequest> result;

        // 遍历请求ID列表，查找匹配的请求
        for (const QString& requestId : requestIds) {
            if (requests.contains(requestId)) {
                result.append(requests[requestId]);  // 如果找到对应请求，添加到结果列表
            }
        }

        return result;  // 返回找到的请求列表
    }

    // 获取符合条件的请求列表
    QList<UrlRequest> getRequestsByFilter(const RequestFilter& filter) {
        QReadLocker locker(&lock);  // 读锁
        QList<UrlRequest> result;

        for (auto it = requests.begin(); it != requests.end(); ++it) {
            const UrlRequest& request = it.value();
            if (filter.matches(request)) {
                result.append(request);
            }
        }
        return result;
    }

    // 删除请求
    void removeRequest(const QString& requestId) 
    {
        QWriteLocker locker(&lock);  // 写锁
        requests.remove(requestId);  // 使用请求ID删除对应的请求
       
    }

    void removeRequest(const QList<QString>* connectm_requestIds) 
    {
        QWriteLocker locker(&lock);  // 写锁
        for (const QString& requestId : *connectm_requestIds) 
        {
            requests.remove(requestId);  // 使用请求ID删除对应的请求
        }

        qDebug() << "缓冲区的请求数量还剩: " << requests.size();
    }




    void removeRequest(const UrlRequest& request) 
    {
        QWriteLocker locker(&lock);  // 写锁
         QString requestId = findRequestIdByUrl(request.url);
         if(requestId.isEmpty())
         {
            return;
         }
        requests.remove(requestId);

    }
    // 删除所有请求
    void removeAllRequests() {
        QWriteLocker locker(&lock);  // 写锁，确保线程安全
        requests.clear();  // 清空所有请求
        qDebug() << "所有请求已删除，缓冲区已清空";
    }

    // 方法1：使用包含关系查找
    bool findUrlInRequests(const QList<UrlRequest>& requests, const QString& urlPart) 
    {
        return std::any_of(requests.begin(), requests.end(),
            [&urlPart](const UrlRequest& request) {
                return request.url.contains(urlPart, Qt::CaseInsensitive);  // 不区分大小写
            });
    }

    // 方法2：返回完整请求信息，支持多个匹配结果
    static QList<UrlRequest> findRequestsByUrlPart(const QList<UrlRequest>& requests, const QString& urlPart) {
        QList<UrlRequest> matches;
        for (const UrlRequest& request : requests) {
            if (request.url.contains(urlPart, Qt::CaseInsensitive)) {
                matches.append(request);
            }
        }
        return matches;
    }


static QList<UrlRequest> findRequestsByUrlPart(
        const QList<UrlRequest>& requests, 
        const QString& urlPart,
        UrlMatchType matchType = UrlMatchType::Contains) 
    {
        QList<UrlRequest> matches;
        for (const UrlRequest& request : requests) {
            bool isMatch = false;
            switch (matchType) {
                case UrlMatchType::Contains:
                    isMatch = request.url.contains(urlPart, Qt::CaseInsensitive);
                    break;
                case UrlMatchType::StartsWith:
                    isMatch = request.url.startsWith(urlPart, Qt::CaseInsensitive);
                    break;
                case UrlMatchType::EndsWith:
                    isMatch = request.url.endsWith(urlPart, Qt::CaseInsensitive);
                    break;
                case UrlMatchType::ExactMatch:
                    isMatch = (request.url.compare(urlPart, Qt::CaseInsensitive) == 0);
                    break;
            }
            if (isMatch) {
                matches.append(request);
            }
        }
        return matches;
    }

    int getRequestCount() const {
        QReadLocker locker(&lock);  // 读锁，允许多个线程并发读取
        return requests.size();  // 返回请求数量
    }


    // 打印所有请求的详细信息
    void printAllRequests() 
    {
        QReadLocker locker(&lock);  // 读锁，允许多个线程并发读取
        for (auto it = requests.begin(); it != requests.end(); ++it) {
            const UrlRequest& request = it.value();
            
            // 将 steps 列表转换为字符串表示
            QString stepsStr;
            for (const RequestStep& step : request.steps) {
                stepsStr += QString::number(static_cast<int>(step)) + " ";  // 将 RequestStep 转为数字并拼接
            }

            qDebug() << "Request ID:" << it.key()
                    << ", URL:" << request.url
                    << ", Running:" << request.isRunning
                    << ", Steps:" << stepsStr.trimmed()  // 去掉末尾的空格
                    << ", Response Format:" << request.responseFormat
                    << ", priority:" << request.priority
                    << ", needParse:" << request.needParse
                    << ", requestType:" << requestTypeToString(request.requestType);
        }
        qDebug() << "缓冲区的请求数量:" << requests.size();
    }



private:
    mutable QReadWriteLock lock;  // 读写锁，保证线程安全
    QHash<QString, UrlRequest> requests;  // 使用 QHash 存储请求，键是请求ID，值是请求的详细信息

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
};


// // 创建一个 UrlRequestManager 实例
// UrlRequestManager manager;

// // 1. 创建单个请求
// UrlRequest request1;
// request1.url = "http://api.example.com/data/1";
// request1.isRunning = true;
// request1.steps = { RequestStep::Step1, RequestStep::Step2 };
// request1.responseFormat = "JSON";
// request1.priority = 10;

// // 添加请求并获取请求ID
// QString requestId1 = manager.addRequest(request1);
// qDebug() << "Added request 1 with ID:" << requestId1;

// // 2. 批量添加多个请求
// UrlRequest request2;
// request2.url = "http://api.example.com/data/2";
// request2.isRunning = false;
// request2.steps = { RequestStep::Step3 };
// request2.responseFormat = "XML";
// request2.priority = 20;

// UrlRequest request3;
// request3.url = "http://api.example.com/data/3";
// request3.isRunning = true;
// request3.steps = { RequestStep::Step2, RequestStep::Step4 };
// request3.responseFormat = "JSON";
// request3.priority = 30;

// QList<UrlRequest> requestList = { request2, request3 };
// QList<QString> requestIds = manager.addRequests(requestList);

// qDebug() << "Added requests with IDs:" << requestIds;

// // 3. 更新某个请求的属性
// qDebug() << "Updating request 1...";
// manager.updateRequest(requestId1, std::nullopt, std::nullopt, std::optional<QList<RequestStep>>{ { RequestStep::Step2, RequestStep::Step3 } }, "XML", 15);

// manager.printAllRequests();
// // 4. 根据过滤条件查询请求

// RequestFilter filter;
// filter.steps = { RequestStep::Step3 };

// qDebug() << "Requests matching filter:";
// QList<UrlRequest> filteredRequests = manager.getRequestsByFilter(filter);

// for (const UrlRequest& req : filteredRequests) {
//     qDebug() << "URL:" << req.url << ", Priority:" << req.priority;
// }

// // 5. 删除一个请求
// qDebug() << "Removing request 2...";
// manager.removeRequest(requestIds[0]);

// // 6. 打印所有请求信息
// manager.printAllRequests();