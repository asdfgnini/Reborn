#pragma once

#include <QString>
#include <QStringList>
#include <QPair>
#include <QVariant>
#include <memory>
#include "URLManger.h"
#include <QDateTime>
#include <QObject>
#include "../Thread/ThreadPool.h"
#include "../Thread/ThreadCommon.h"
#include <QThread>
#include "UrlConfig.h"





// URL生成器基类
class BaseUrlGenerator {
protected:
    QString generateUrl(const QStringList& pathParts, 
                       const QList<QPair<QString, QVariant>>& params) {
        QString url = QString("http://%1:%2").arg(m_ip).arg(m_port);
        
        // 添加路径
        for (const QString& part : pathParts) {
            url += "/" + part;
        }
        
        // 添加参数
        if (!params.isEmpty()) {
            url += "?";
            QStringList paramStrings;
            for (const auto& param : params) {
                paramStrings << QString("%1=%2").arg(param.first)
                                              .arg(param.second.toString());
            }
            url += paramStrings.join("&");
        }
        
        return url;
    }
    QList<RequestStep> parseSteps(const QString& stepsStr) {
        QList<RequestStep> steps;
        QStringList stepsList = stepsStr.split(",", Qt::SkipEmptyParts);
        
        for (const QString& step : stepsList) {
            if (step == "Step1") steps.append(RequestStep::Step1);
            else if (step == "Step2") steps.append(RequestStep::Step2);
            else if (step == "Step3") steps.append(RequestStep::Step3);
            else if (step == "Step4") steps.append(RequestStep::Step4);
            else if (step == "Step5") steps.append(RequestStep::Step5);
        }
        
        return steps;
    }
    // 修改方法签名，添加记录列表参数
    virtual bool addSpecificRequests(const ConfigFilter& filter, 
                                   UrlRequestManager* manager,
                                   QList<UrlRequest>& requestRecord) = 0;

    QString m_ip;
    int m_port;

protected:
    void setRequestProperties(UrlRequest& request, const QList<QPair<QString, QVariant>>& bufferParams) {
        // ... 设置请求属性的代码 ...
    }
};

// ... 保持 UrlConfig 的其他部分不变 ...

class ConnectUrlGenerator : public BaseUrlGenerator {
public:
    explicit ConnectUrlGenerator(const QString& ip, int port) {
        m_ip = ip;
        m_port = port;
        qDebug() << "Created ConnectUrlGenerator with IP:" << ip << "Port:" << port;
    }

QList<UrlRequest> generateRequests() {
    qDebug() << "ConnectUrlGenerator: Starting to generate requests...";
    QList<UrlRequest> requests;
    auto configs = UrlConfig::getConnectConfigs();
    
    for (const auto& config : configs) {
        if(config.addToBufferImmediately)
        {
            UrlRequest request;
            request.url = generateUrl(config.pathParts, config.params);
            request.requestType = config.requestType;
            request.parsePaths = config.parsePaths;  // 直接赋值 QMap
            request.needParse = !config.parsePaths.isEmpty();
            
            // qDebug() << "ConnectUrlGenerator: Generated URL:" << request.url;
            
            // 从 bufferParams 设置请求属性
            for (const auto& param : config.bufferParams) {
                if (param.first == "isRunning") {
                    request.isRunning = param.second.toString().toLower() == "true";
                    // qDebug() << "ConnectUrlGenerator: Set isRunning:" << request.isRunning;
                } 
                else if (param.first == "steps") {
                    request.steps = parseSteps(param.second.toString());
                    // qDebug() << "ConnectUrlGenerator: Set steps:" << param.second.toString();
                }
                else if (param.first == "responseFormat") {
                    request.responseFormat = param.second.toString();
                    // qDebug() << "ConnectUrlGenerator: Set responseFormat:" << request.responseFormat;
                }
                else if (param.first == "priority") {
                    request.priority = param.second.toInt();
                    // qDebug() << "ConnectUrlGenerator: Set priority:" << request.priority;
                }
            }

            // 打印解析路径信息
            // if (request.needParse) {
            //     qDebug() << "ConnectUrlGenerator: Parse paths for" << request.url;
            //     for (auto it = request.parsePaths.constBegin(); it != request.parsePaths.constEnd(); ++it) {
            //         qDebug() << "  Key:" << it.key()
            //                 << "Path:" << it.value().key
            //                 << "Description:" << it.value().description
            //                 << "Type:" << it.value().dataType
            //                 << "Default:" << it.value().defaultValue;
            //     }
            // }
            
            requests.append(request);
            // qDebug() << "ConnectUrlGenerator: Added request with type:" 
            //          << requestTypeToString(request.requestType);
        }
    }
    
    qDebug() << "ConnectUrlGenerator: Generated" << requests.size() << "requests";
    return requests;
}
    bool addSpecificRequests(const ConfigFilter& filter, 
                            UrlRequestManager* manager,
                            QList<UrlRequest>& requestRecord) override {
        bool added = false;
        auto configs = UrlConfig::getConnectConfigs();
        
        for (const auto& config : configs) {
            if (!config.addToBufferImmediately && filter.matches(config)) {
                qDebug() << "Found matching connect config:" << config.pathParts.join("/");
                UrlRequest request;
                request.url = generateUrl(config.pathParts, config.params);
                setRequestProperties(request, config.bufferParams);
                
                // 添加到管理器
                manager->addRequest(request);
                // 添加到记录列表
                requestRecord.append(request);
                added = true;
            }
        }
        
        if (added) {
            qDebug() << "Added" << requestRecord.size() << "connect requests";
        }
        return added;
    }
    void generateSpecificRequests(const ConfigFilter& filter, 
                                UrlRequestManager* manager,
                                QList<UrlRequest>& requests) {
        bool added = false;
        auto configs = UrlConfig::getConnectConfigs();
        
        for (const auto& config : configs) {
            if (!config.addToBufferImmediately && filter.matches(config)) {
                UrlRequest request;
                request.url = generateUrl(config.pathParts, config.params);
                setRequestProperties(request, config.bufferParams);
                requests.append(request);
            }
        }
    }   
private:

    void setRequestProperties(UrlRequest& request, const QList<QPair<QString, QVariant>>& bufferParams) {
        for (const auto& param : bufferParams) {
            if (param.first == "isRunning") {
                request.isRunning = param.second.toString().toLower() == "true";
                qDebug() << "Set isRunning:" << request.isRunning;
            } 
            else if (param.first == "steps") {
                request.steps = parseSteps(param.second.toString());
                qDebug() << "Set steps:" << param.second.toString();
            } 
            else if (param.first == "responseFormat") {
                request.responseFormat = param.second.toString();
                qDebug() << "Set responseFormat:" << request.responseFormat;
            } 
            else if (param.first == "priority") {
                request.priority = param.second.toInt();
                qDebug() << "Set priority:" << request.priority;
            }
            // needParse 现在由 parsePaths 是否为空决定
            request.needParse = !request.parsePaths.isEmpty();
        }
        
        // 如果有解析路径，打印调试信息
        if (request.needParse) {
            qDebug() << "Parse paths for URL:" << request.url;
            for (auto it = request.parsePaths.constBegin(); it != request.parsePaths.constEnd(); ++it) {
                qDebug() << "  Key:" << it.key()
                        << "Path:" << it.value().key
                        << "Description:" << it.value().description
                        << "Type:" << it.value().dataType
                        << "Default:" << it.value().defaultValue;
            }
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
};

class ScanUrlGenerator : public BaseUrlGenerator {
public:
    explicit ScanUrlGenerator(const QString& ip, int port) {
        m_ip = ip;
        m_port = port;
        qDebug() << "Created ScanUrlGenerator with IP:" << ip << "Port:" << port;
    }

    void setScanParams(const QString& startMass, const QString& stopMass, 
                      const QString& ppamu, const QString& dwell) {
        m_startMass = startMass;
        m_stopMass = stopMass;
        m_ppamu = ppamu;
        m_dwell = dwell;
        qDebug() << "ScanUrlGenerator: Set scan params -"
                 << "StartMass:" << startMass
                 << "StopMass:" << stopMass
                 << "PPAMU:" << ppamu
                 << "Dwell:" << dwell;
    }

QList<UrlRequest> generateRequests() {
    qDebug() << "ScanUrlGenerator: Starting to generate requests...";
    QList<UrlRequest> requests;
    auto configs = UrlConfig::getScanConfigs();
    
    for (auto& config : configs) {
        // 只为需要扫描参数的URL添加这些参数
        if (config.needScanParams) {
            qDebug() << "ScanUrlGenerator: Adding scan parameters to URL";
            config.params.append({
                {"startMass", m_startMass},
                {"stopMass", m_stopMass},
                {"ppamu", m_ppamu},
                {"dwell", m_dwell}
            });
        }
        
        UrlRequest request;
        request.url = generateUrl(config.pathParts, config.params);
        request.requestType = config.requestType;
        request.parsePaths = config.parsePaths;  // 直接赋值 QMap
        request.needParse = !config.parsePaths.isEmpty();
        
        // qDebug() << "ScanUrlGenerator: Generated URL:" << request.url;
        
        // 从 bufferParams 设置请求属性
        for (const auto& param : config.bufferParams) {
            if (param.first == "isRunning") {
                request.isRunning = param.second.toString().toLower() == "true";
                // qDebug() << "ScanUrlGenerator: Set isRunning:" << request.isRunning;
            } 
            else if (param.first == "steps") {
                request.steps = parseSteps(param.second.toString());
                // qDebug() << "ScanUrlGenerator: Set steps:" << param.second.toString();
            }
            else if (param.first == "responseFormat") {
                request.responseFormat = param.second.toString();
                // qDebug() << "ScanUrlGenerator: Set responseFormat:" << request.responseFormat;
            }
            else if (param.first == "priority") {
                request.priority = param.second.toInt();
                // qDebug() << "ScanUrlGenerator: Set priority:" << request.priority;
            }
        }

        // 打印解析路径信息
        // if (request.needParse) {
        //     qDebug() << "ScanUrlGenerator: Parse paths for" << request.url;
        //     for (auto it = request.parsePaths.constBegin(); it != request.parsePaths.constEnd(); ++it) {
        //         qDebug() << "  Key:" << it.key()
        //                 << "Path:" << it.value().key
        //                 << "Description:" << it.value().description
        //                 << "Type:" << it.value().dataType
        //                 << "Default:" << it.value().defaultValue;
        //     }
        // }
        
        requests.append(request);
        // qDebug() << "ScanUrlGenerator: Added request with type:" 
        //          << requestTypeToString(request.requestType);

    }
    
    qDebug() << "ScanUrlGenerator: Generated" << requests.size() << "requests";
    return requests;
}
    bool addSpecificRequests(const ConfigFilter& filter, 
                            UrlRequestManager* manager,
                            QList<UrlRequest>& requestRecord) override {
        bool added = false;
        auto configs = UrlConfig::getScanConfigs();
        
        for (const auto& config : configs) {
            if (!config.addToBufferImmediately && filter.matches(config)) {
                qDebug() << "Found matching scan config:" << config.pathParts.join("/");
                UrlRequest request;
                request.url = generateUrl(config.pathParts, config.params);
                setRequestProperties(request, config.bufferParams);
                
                // 添加到管理器
                manager->addRequest(request);
                // 添加到记录列表
                requestRecord.append(request);
                added = true;
            }
        }
        
        if (added) {
            qDebug() << "Added" << requestRecord.size() << "scan requests";
        }
        return added;
    }

    void generateSpecificRequests(const ConfigFilter& filter, 
                                UrlRequestManager* manager,
                                QList<UrlRequest>& requests) {
         bool added = false;
        auto configs = UrlConfig::getScanConfigs();

        
        for (const auto& config : configs) {
            if (!config.addToBufferImmediately && filter.matches(config)) {
                qDebug() << "Found matching scan config:" << config.pathParts.join("/");
                UrlRequest request;
                request.url = generateUrl(config.pathParts, config.params);
                setRequestProperties(request, config.bufferParams);
                
                // 添加到管理器
                manager->addRequest(request);
                // 添加到记录列表
                requests.append(request);
            }
        }
    }
private:
    void setRequestProperties(UrlRequest& request, const QList<QPair<QString, QVariant>>& bufferParams) {
        for (const auto& param : bufferParams) {
            if (param.first == "isRunning") {
                request.isRunning = param.second.toString().toLower() == "true";
                qDebug() << "Set isRunning:" << request.isRunning;
            } 
            else if (param.first == "steps") {
                request.steps = parseSteps(param.second.toString());
                qDebug() << "Set steps:" << param.second.toString();
            } 
            else if (param.first == "responseFormat") {
                request.responseFormat = param.second.toString();
                qDebug() << "Set responseFormat:" << request.responseFormat;
            } 
            else if (param.first == "priority") {
                request.priority = param.second.toInt();
                qDebug() << "Set priority:" << request.priority;
            }
            // needParse 现在由 parsePaths 是否为空决定
            request.needParse = !request.parsePaths.isEmpty();
        }
        
        // 如果有解析路径，打印调试信息
        if (request.needParse) {
            qDebug() << "Parse paths for URL:" << request.url;
            for (auto it = request.parsePaths.constBegin(); it != request.parsePaths.constEnd(); ++it) {
                qDebug() << "  Key:" << it.key()
                        << "Path:" << it.value().key
                        << "Description:" << it.value().description
                        << "Type:" << it.value().dataType
                        << "Default:" << it.value().defaultValue;
            }
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

private:
    QString m_startMass;
    QString m_stopMass;
    QString m_ppamu;
    QString m_dwell;
};


class UrlGenerator : public QObject {
    Q_OBJECT
public:
    explicit UrlGenerator(const QString& ip = "192.168.1.101", int port = 80)
        : m_connectGenerator(std::make_unique<ConnectUrlGenerator>(ip, port))
        , m_scanGenerator(std::make_unique<ScanUrlGenerator>(ip, port))
        , m_requestManager(std::make_unique<UrlRequestManager>())
    {}
// 添加特定的连接请求
    bool addSpecificConnectRequests(const ConfigFilter& filter) {
        return m_connectGenerator->addSpecificRequests(filter, 
                                                     m_requestManager.get(), 
                                                     m_connectRequests);
    }

    // 添加特定的扫描请求
    bool addSpecificScanRequests(const ConfigFilter& filter) {
        return m_scanGenerator->addSpecificRequests(filter, 
                                                  m_requestManager.get(), 
                                                  m_scanRequests);
    }

    // 添加所有类型的特定请求
    bool addAllSpecificRequests(const ConfigFilter& filter) {
        bool connectAdded = addSpecificConnectRequests(filter);
        bool scanAdded = addSpecificScanRequests(filter);
        return connectAdded || scanAdded;
    }

    // 获取特定的连接请求
    QList<UrlRequest> getSpecificConnectRequests(const ConfigFilter& filter) {
        QList<UrlRequest> requests;
        m_connectGenerator->generateSpecificRequests(filter, m_requestManager.get(), requests);
        return requests;
    }

    // 获取特定的扫描请求
    QList<UrlRequest> getSpecificScanRequests(const ConfigFilter& filter) {
        QList<UrlRequest> requests;
        m_scanGenerator->generateSpecificRequests(filter, m_requestManager.get(), requests);
        return requests;
    }


    QList<UrlRequest> getAllRequests() const {
        return m_requestManager->getAllRequests();
    }

    QList<UrlRequest> getRequestsByFilter(const RequestFilter& filter) {
        return m_requestManager->getRequestsByFilter(filter);
    }

    // void updateRequest(const UrlRequest& request,
    //                   const std::optional<bool>& newIsRunning = std::nullopt,
    //                   const std::optional<QList<RequestStep>>& newSteps = std::nullopt,
    //                   const std::optional<QString>& newResponseFormat = std::nullopt,
    //                   const std::optional<int>& newPriority = std::nullopt) {
    //     m_requestManager->updateRequest(request, newIsRunning, newSteps, 
    //                                  newResponseFormat, newPriority);
    // }

    // void updateRequestsByFilter(const RequestFilter& filter,
    //                           const std::optional<bool>& newIsRunning = std::nullopt,
    //                           const std::optional<QList<RequestStep>>& newSteps = std::nullopt,
    //                           const std::optional<QString>& newResponseFormat = std::nullopt,
    //                           const std::optional<int>& newPriority = std::nullopt) {
    //     m_requestManager->updateRequestsByFilter(filter, newIsRunning, newSteps,
    //                                           newResponseFormat, newPriority);
    // }

    // 查找特定URL的请求
    QList<UrlRequest> findRequestsByUrlPart(const QString& urlPart, 
                                          UrlMatchType matchType = UrlMatchType::Contains) {
        return UrlRequestManager::findRequestsByUrlPart(getAllRequests(), urlPart, matchType);
    }

    void removeRequests(const QList<UrlRequest>& requests) {
        for (const auto& request : requests) {
            m_requestManager->removeRequest(request);
        }
    }

    // 打印所有请求信息
    void printAllRequests() {
        m_requestManager->printAllRequests();
    }
     // 打印当前记录的请求信息
    void printRequestRecords() const {
        qDebug() << "\n=== Connect Requests ===";
        qDebug() << "Total:" << m_connectRequests.size();
        for (const auto& req : m_connectRequests) 
        {
            qDebug() << "URL:" << req.url;
        }

        qDebug() << "\n=== Scan Requests ===";
        qDebug() << "Total:" << m_scanRequests.size();
        for (const auto& req : m_scanRequests) {
            qDebug() << "URL:" << req.url;
        }
    }

public slots:
    void startConnect(QString ip, int port, bool checked) {
        qDebug() << "UrlController: Starting connect operation";
        if (checked) {
            m_connectGenerator = std::make_unique<ConnectUrlGenerator>(ip, port);
            
            ThreadPool::getInstance()->work(
                [this]() {
                    auto requests = m_connectGenerator->generateRequests();
                    m_connectRequests.clear();  // 清除旧的记录
                    for (const auto& request : requests) {
                        m_requestManager->addRequest(request);
                        m_connectRequests.append(request);  // 记录添加的请求
                    }
                    return !requests.isEmpty();
                },
                [this](bool success) {
                    qDebug() << " 缓冲区请求数量:" << m_requestManager->getRequestCount();
                    if (success) {
                        emit dataUpdated();
                    }
                }
            );
        }
    }

    void startScan(QString startMass, QString stopMass, QString ppamu, 
                   QString dwell, bool scanState) {
        qDebug() << "UrlController: Starting scan operation";
        if (scanState) {
            m_scanGenerator->setScanParams(startMass, stopMass, ppamu, dwell);
            
            ThreadPool::getInstance()->work(
                [this]() {
                    auto requests = m_scanGenerator->generateRequests();
                    m_scanRequests.clear();  // 清除旧的记录
                    for (const auto& request : requests) {
                        m_requestManager->addRequest(request);
                        m_scanRequests.append(request);  // 记录添加的请求
                    }
                    return !requests.isEmpty();
                },
                [this,startMass,stopMass,ppamu,dwell](bool success) {
                    qDebug() << " 缓冲区请求数量:" << m_requestManager->getRequestCount();                    
                    if (success) {
                        emit dataScanUpdated(startMass, stopMass, ppamu, dwell);
                    }
                }
            );
        }
    }

    void stopConnect() 
    {
        qDebug() << "UrlController: Stopping connect operation";
        // 直接删除记录的请求
        for (const auto& request : m_connectRequests) {
            m_requestManager->removeRequest(request);
        }
        m_connectRequests.clear();
    }

    void stopScan() 
    {
        qDebug() << "UrlController: Stopping scan operation";
        // 直接删除记录的请求
        for (const auto& request : m_scanRequests) {
            m_requestManager->removeRequest(request);
        }
        m_scanRequests.clear();
    }

signals:
    void dataUpdated();
    void dataScanUpdated(QString startMass, QString stopMass, 
                        QString ppamu, QString dwell);

private:
    std::unique_ptr<ConnectUrlGenerator> m_connectGenerator;
    std::unique_ptr<ScanUrlGenerator> m_scanGenerator;
    std::unique_ptr<UrlRequestManager> m_requestManager;
    
    QList<UrlRequest> m_connectRequests;  // 记录连接相关的请求
    QList<UrlRequest> m_scanRequests;     // 记录扫描相关的请求

};


// // 创建过滤器并直接添加匹配的请求
// ConfigFilter filter;
// filter.withPathParts({"scanSetup", "channels"})
//       .withBufferParamKeys({"steps"})
//       .withBufferParamValues({"Step2"});

// // 添加特定类型的请求
// if (urlGenerator.addSpecificConnectRequests(filter)) {
//     qDebug() << "Successfully added connect requests";
// }

// if (urlGenerator.addSpecificScanRequests(filter)) {
//     qDebug() << "Successfully added scan requests";
// }

// // 或者添加所有类型的请求
// if (urlGenerator.addAllSpecificRequests(filter)) {
//     qDebug() << "Successfully added all matching requests";
// }


