#include "DeviceMonitor.h"
#include "URLManger.h"
#include <qcontainerfwd.h>
#include "DeviceStateManager.h"


DeviceMonitor::DeviceMonitor(QObject *parent,UrlGenerator* urlGenerator) : MyThread(parent)
    , m_urlGenerator(urlGenerator)
    
{
    setissequence(false);

    RequestFilter filter;
    filter.steps = { RequestStep::Invalid };
    setm_filter(filter);

    setisFirstConnect(false);


}

DeviceMonitor::~DeviceMonitor()
{
    qDebug() << "DeviceMonitor::~DeviceMonitor()";
    stopThread();
}

void DeviceMonitor::processTask()
{
    QList<UrlRequest> requests = m_urlGenerator->getRequestsByFilter(getm_filter());
    // qDebug() << "DeviceMonitor::processTask requests size" << requests.size();
    QString retStr;
    QString err;

    for(const UrlRequest& request : requests)
    {
        // qDebug() << "request url:" << request.url;

        // QString url = "https://jsonplaceholder.typicode.com/todos/1";
        // HttpRequestSync("http://jsonplaceholder.typicode.com/posts/1", retStr, err, 3000, ResponseFormat::JSON);
        // HttpRequestSync("http://httpbin.org/get", retStr, err, 3000, ResponseFormat::JSON);
        // HttpRequestSync("https://reqres.in/api/users?page=2", retStr, err, 3000, ResponseFormat::JSON);
        // HttpRequestSync("https://jsonplaceholder.typicode.com/todos/1", retStr, err, 3000, ResponseFormat::JSON);
        if(HttpRequestSync(request.url, retStr, err, 3000, ResponseFormat::JSON))
        {
            if(request.needParse)
            {
                bool ok;
                QJsonObject jsonObj = parseJsonString(retStr, &ok);
                if (ok && !request.parsePaths.isEmpty()) 
                {
                    switch(request.requestType) {
                        case RequestType::ScanStatus: {
                            ScanStatus status;
                            status.firstScan = getJsonValue(jsonObj, request.parsePaths["firstScan"].key,0).toUInt();
                            status.lastScan = getJsonValue(jsonObj, request.parsePaths["lastScan"].key,0).toUInt();
                            status.pointsInCurrentScan = getJsonValue(jsonObj, request.parsePaths["pointsInCurrentScan"].key,0).toUInt();
                            status.pointsPerScan = getJsonValue(jsonObj, request.parsePaths["pointsPerScan"].key,0).toUInt();
                            status.currentScan = getJsonValue(jsonObj, request.parsePaths["currentScan"].key,0).toUInt();
                            status.scanning = getJsonValue(jsonObj, request.parsePaths["scanning"].key,false).toString().toLower() == "true";
                            DeviceStateManager::instance()->updateScanStatus(DEVICE_ID0, status);
                            break;
                        }
                        case RequestType::ControlInfo: {
                            ControlInfo info;
                            info.amInControl = getJsonValue(jsonObj, request.parsePaths["amInControl"].key,false).toBool();
                            info.canForceControl = getJsonValue(jsonObj, request.parsePaths["canForceControl"].key,false).toBool();
                            info.controlled = getJsonValue(jsonObj, request.parsePaths["controlled"].key,false).toBool();
                            info.locked = getJsonValue(jsonObj, request.parsePaths["locked"].key,false).toBool();
                            info.secondsSinceLastControlRequest = getJsonValue(jsonObj, request.parsePaths["secondsSinceLastControlRequest"].key,0).toUInt();
                            info.secondsSinceLastDataRequest = getJsonValue(jsonObj, request.parsePaths["secondsSinceLastDataRequest"].key,0).toUInt();
                            info.secondsSinceLastRequest = getJsonValue(jsonObj, request.parsePaths["secondsSinceLastRequest"].key,0).toUInt();
                            DeviceStateManager::instance()->updateControlInfo(DEVICE_ID0, info);
                            break;
                        }
                        default:
                            break;

                    }
                }
            }
        }
    }

    QThread::msleep(1500);
}   

void DeviceMonitor::setissequence(bool issequence)
{
    QMutexLocker locker(&m_mutex);
    this->issequence = issequence;
}

bool DeviceMonitor::getissequence()
{
    QMutexLocker locker(&m_mutex);
    return issequence;
}


bool DeviceMonitor::HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, int timeout, ResponseFormat format)
{
    QUrl url(urlStr);
    QNetworkRequest request(url);

    // 根据传入的 format 参数设置请求头
    switch (format) {
        case ResponseFormat::JSON:
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            request.setRawHeader("Accept", "application/json");  // 使用 setRawHeader 设置 Accept header
            break;
        case ResponseFormat::XML:
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");
            request.setRawHeader("Accept", "application/xml");  // 使用 setRawHeader 设置 Accept header
            break;
        case ResponseFormat::PlainText:
            request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
            request.setRawHeader("Accept", "text/plain");  // 使用 setRawHeader 设置 Accept header
            break;
        case ResponseFormat::Binary:
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
            request.setRawHeader("Accept", "application/octet-stream");  // 使用 setRawHeader 设置 Accept header
            break;
    }

    QNetworkAccessManager httpClient;
    QNetworkReply* reply = httpClient.get(request);

    bool okFinish = false;
    bool okTimeout = false;
    
    QEventLoop loop; // 自定义事件循环
    QTimer timer;    // 用于超时退出

    // 响应处理
    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        timer.stop(); // 停止超时计时器
        if(reply->error() == QNetworkReply::NoError) {
            retStr = reply->readAll(); // 获取响应内容
            okFinish = true; // 请求成功
        } else {
            err = reply->errorString();
        }
        loop.quit(); // 退出事件循环
    });

    // 超时处理
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        okTimeout = true;
        err = "Request timed out";
        reply->abort(); // 终止请求
        loop.quit();    // 退出事件循环
    });

    timer.setSingleShot(true);
    timer.start(timeout); // 设置超时时间

    loop.exec(); // 进入事件循环，等待请求完成或超时

    reply->deleteLater(); // 确保资源释放

    // 解析返回数据（根据格式不同进行处理）
    if (okFinish && !okTimeout) {
        switch (format) {
            case ResponseFormat::JSON: 
                // 处理 JSON 数据
                break;
            case ResponseFormat::XML:
                // 处理 XML 数据（可用 QDomDocument 进行解析）
                break;
            case ResponseFormat::PlainText:
                // 处理纯文本数据
                break;
            case ResponseFormat::Binary:
                // 处理二进制数据
                break;
        }
    }

    return okFinish && !okTimeout; // 返回请求是否成功且没有超时
}


QList<UrlRequest> DeviceMonitor::sortRequestsByPriority(const QList<UrlRequest>& requests) 
{
    // 打印排序前的请求列表
    qDebug() << "排序前的请求列表:";
    for (const UrlRequest& request : requests) {
        qDebug() << "URL:" << request.url << "Priority:" << request.priority;
    }

    // 创建一个副本来排序
    QList<UrlRequest> sortedRequests = requests;

    // 按照优先级 (priority) 排序，优先级高的排前面
    std::sort(sortedRequests.begin(), sortedRequests.end(), [](const UrlRequest& a, const UrlRequest& b) {
        return a.priority > b.priority;  // 优先级从高到低排序
    });

    // 打印排序后的请求列表
    qDebug() << "排序后的请求列表:";
    for (const UrlRequest& request : sortedRequests) {
        qDebug() << "URL:" << request.url << "Priority:" << request.priority;
    }

    return sortedRequests;
}

void DeviceMonitor::startconnect()
{

    if (this->getState() == MyThread::ThreadState::Stopped) 
    {
        RequestFilter filter;
        filter.steps = { RequestStep::Step3};
        setm_filter(filter);

        this->startThread();

    }
    else if (this->getState() == MyThread::ThreadState::Paused) 
    {
        this->resumeThread();
    }
}

void DeviceMonitor::stopconnect()
{

    if(this->getState() == MyThread::ThreadState::Running)
    {
        this->stopThread();
        
        RequestFilter filter;
        filter.steps = { RequestStep::Invalid };
        setm_filter(filter);

        emit stopconnectSignal();
    }
}


void DeviceMonitor::setm_filter(const RequestFilter& filter)
{
    QMutexLocker locker(&m_mutex);
    this->m_filter = filter;
}

RequestFilter DeviceMonitor::getm_filter()
{
    QMutexLocker locker(&m_mutex);
    return m_filter;
}

void DeviceMonitor::setisFirstConnect(bool isFirstConnect)
{
    QMutexLocker locker(&m_mutex);
    this->isFirstConnect = isFirstConnect;

}
bool DeviceMonitor::getisFirstConnect()
{
    QMutexLocker locker(&m_mutex);
    return isFirstConnect;

}

QJsonObject DeviceMonitor::parseJsonString(const QString& jsonString, bool* ok)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
    
    if (ok != nullptr) {
        *ok = (error.error == QJsonParseError::NoError && doc.isObject());
    }
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON解析错误:" << error.errorString();
        return QJsonObject();
    }
    
    return doc.object();
}

QStringList DeviceMonitor::splitJsonPath(const QString& path)
{
    return path.split(".", Qt::SkipEmptyParts);
}

QVariant DeviceMonitor::getJsonValue(const QJsonObject& json, const QString& path, const QVariant& defaultValue)
{
    QStringList pathParts = splitJsonPath(path);
    QJsonValue currentValue = json;
    
    for (const QString& key : pathParts) {
        if (currentValue.isObject()) {
            if (!currentValue.toObject().contains(key)) {
                qDebug() << "JSON路径不存在:" << path;
                return defaultValue;
            }
            currentValue = currentValue.toObject().value(key);
        } else if (currentValue.isArray()) {
            bool ok;
            int index = key.toInt(&ok);
            if (!ok || index < 0 || index >= currentValue.toArray().size()) {
                qDebug() << "无效的数组索引:" << key;
                return defaultValue;
            }
            currentValue = currentValue.toArray().at(index);
        } else {
            qDebug() << "无效的JSON路径:" << path;
            return defaultValue;
        }
    }
    
    if (currentValue.isString()) return currentValue.toString();
    if (currentValue.isDouble()) return currentValue.toDouble();
    if (currentValue.isBool()) return currentValue.toBool();
    if (currentValue.isArray()) return QVariant::fromValue(currentValue.toArray());
    if (currentValue.isObject()) return QVariant::fromValue(currentValue.toObject());
    if (currentValue.isNull()) return QVariant();
    
    return defaultValue;
}



// void DeviceMonitor::processTask()
// {
//     // ... existing code ...

//     for(const UrlRequest& request : requests)
//     {
//         // ... existing code ...

//         if(HttpRequestSync("https://postman-echo.com/get?foo1=bar1&foo2=bar2", retStr, err, 3000, ResponseFormat::JSON))
//         {
//             if(request.needParse)
//             {
//                 bool ok;
//                 QJsonObject jsonObj = parseJsonString(retStr, &ok);
//                 if (ok) 
//                 {
//                     // 根据请求类型更新不同的状态
//                     switch(request.requestType) {
//                         case RequestType::ScanStatus: {
//                             ScanStatus status;
//                             status.deviceId = getJsonValue(jsonObj, "deviceId").toString();
//                             status.isScanning = getJsonValue(jsonObj, "isScanning").toBool();
//                             status.scanProgress = getJsonValue(jsonObj, "scanProgress").toInt();
//                             status.lastScanTime = QDateTime::fromString(
//                                 getJsonValue(jsonObj, "lastScanTime").toString(),
//                                 Qt::ISODate
//                             );
//                             DeviceStateManager::instance()->updateScanStatus(status.deviceId, status);
//                             break;
//                         }
//                         case RequestType::ConnectionStatus: {
//                             ConnectionStatus status;
//                             status.deviceId = getJsonValue(jsonObj, "deviceId").toString();
//                             status.isConnected = getJsonValue(jsonObj, "isConnected").toBool();
//                             status.connectionType = getJsonValue(jsonObj, "connectionType").toString();
//                             status.signalStrength = getJsonValue(jsonObj, "signalStrength").toInt();
//                             DeviceStateManager::instance()->updateConnectionStatus(status.deviceId, status);
//                             break;
//                         }
//                         case RequestType::SensorStatus: {
//                             SensorStatus status;
//                             status.deviceId = getJsonValue(jsonObj, "deviceId").toString();
//                             status.temperature = getJsonValue(jsonObj, "temperature").toDouble();
//                             status.humidity = getJsonValue(jsonObj, "humidity").toDouble();
//                             status.pressure = getJsonValue(jsonObj, "pressure").toDouble();
//                             status.isCalibrated = getJsonValue(jsonObj, "isCalibrated").toBool();
//                             DeviceStateManager::instance()->updateSensorStatus(status.deviceId, status);
//                             break;
//                         }
//                         case RequestType::SystemStatus: {
//                             SystemStatus status;
//                             status.deviceId = getJsonValue(jsonObj, "deviceId").toString();
//                             status.firmwareVersion = getJsonValue(jsonObj, "firmwareVersion").toString();
//                             status.batteryLevel = getJsonValue(jsonObj, "batteryLevel").toInt();
//                             status.isCharging = getJsonValue(jsonObj, "isCharging").toBool();
//                             status.workMode = getJsonValue(jsonObj, "workMode").toString();
//                             DeviceStateManager::instance()->updateSystemStatus(status.deviceId, status);
//                             break;
//                         }
//                         case RequestType::DeviceBaseInfo: {
//                             DeviceBaseInfo info;
//                             info.deviceId = getJsonValue(jsonObj, "deviceId").toString();
//                             info.startChannel = getJsonValue(jsonObj, "startChannel").toInt();
//                             info.stopChannel = getJsonValue(jsonObj, "stopChannel").toInt();
//                             info.scanCount = getJsonValue(jsonObj, "scanCount").toInt();
//                             info.scanInterval = getJsonValue(jsonObj, "scanInterval").toInt();
//                             info.leakCheckMass = getJsonValue(jsonObj, "leakCheckMass").toInt();
//                             info.scanTimeTotal = getJsonValue(jsonObj, "scanTimeTotal").toInt();
//                             info.captureFileStatus = getJsonValue(jsonObj, "captureFileStatus").toInt();
//                             DeviceStateManager::instance()->updateDeviceBaseInfo(info.deviceId, info);
//                             break;
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     QThread::msleep(1500);
// }