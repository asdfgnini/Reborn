
#include "UrlGenerator.h"
#include <qmutex.h>

UrlGenerator::UrlGenerator(const QString& ip, int port)
    : _pRGAip(ip), _pRGAport(port)
{
    //状态初始化
    isConnect = false;
    isScan = false;
    isConnectDevice = false;
    isrepeatconnect = false;
    isrepeatscan = false;
    QString m_startMass = "0";
    QString m_stopMass = "200";
    QString m_ppamu = "1";
    QString m_dwell = "8";

//URL生成
    m_urlConfigs = new QList<QMap<QString, QVariant>>();
    m_urlconnectConfigstosave = new QList<QMap<QString, QVariant>>();
    m_urlscanConfigstosave = new QList<QMap<QString, QVariant>>();
    m_connectuniqueUrls = new QSet<QString>();
    m_scanuniqueUrls = new QSet<QString>();
    connectm_requestIds = new QList<QString>();
    Scanm_requestIds = new QList<QString>();
    requestManager = new UrlRequestManager();
}

UrlGenerator::~UrlGenerator()
{
    m_connectuniqueUrls->clear();
    m_scanuniqueUrls->clear();
    m_urlConfigs->clear();
    m_urlconnectConfigstosave->clear();
    m_urlscanConfigstosave->clear();
    connectm_requestIds->clear();
    Scanm_requestIds->clear();
    requestManager->removeAllRequests();

    delete m_urlConfigs;
    delete m_urlconnectConfigstosave;
    delete m_urlscanConfigstosave;
    delete m_connectuniqueUrls;
    delete m_scanuniqueUrls;
    delete connectm_requestIds;
    delete Scanm_requestIds;
    delete requestManager;
}

void UrlGenerator::startconnect(QString ip, int port,bool checked)
{
    setRGAip(ip);
    setRGAport(port);
    setConnectStatus(checked);
#ifdef TAO_DEBUG
    // qDebug() << "UrlGenerator: "<< "ip:" << getRGAip() << "port:" << getRGAport();
#endif

    // 绑定工作任务和结果回调
    WorkCallback workCall = std::bind(&UrlGenerator::GenUrlDatawork, this); // 绑定成员函数 doWork
    WorkResultCallback resultCall = std::bind(&UrlGenerator::GenUrlDataresult, this, std::placeholders::_1); // 绑定成员函数 handleResult

    // 使用线程池执行工作
    ThreadPool::getInstance()->work(workCall, resultCall);
}

void UrlGenerator::stopconnect()
{
    m_connectuniqueUrls->clear();
     m_scanuniqueUrls->clear();
    m_urlConfigs->clear();
    m_urlconnectConfigstosave->clear();
    m_urlscanConfigstosave->clear();
    connectm_requestIds->clear();
    Scanm_requestIds->clear();
    
    requestManager->removeAllRequests();
}

bool UrlGenerator::GenUrlDatawork()
{
#ifdef TAO_DEBUG
    qDebug() << "数据准备线程 is running: " << QThread::currentThreadId();
#endif

    addconnectUrlConfig({"mmsp", "scanInfo", "get"}, {},{{"isRunning", false},{"step","Step1"}});   
    addconnectUrlConfig({"mmsp", "communication", "controlInfo", "get"}, {}, {{"isRunning", true},{"step","Step1"}});

    if(isrepeatconnect == false)
    {
        // 遍历配置列表生成 URL 请求
        for (const auto& config : *m_urlconnectConfigstosave)
        {
#ifdef TAO_DEBUG
            qDebug() << "UrlRequest URL: " << config["path"].toString();
#endif
            // 创建 UrlRequest 对象
            UrlRequest request;
            request.url = config["path"].toString();
            
            if (config.contains("isRunning")) 
            {
                request.isRunning = config["isRunning"].toBool();
            }

            if (config.contains("step")) 
            {
                int stepValue = config["step"].toInt();  // 获取 step 的整数值

                // 检查该值是否在合法的范围内（1 到 5），然后转换为枚举类型
                if (stepValue >= 1 && stepValue <= 5) 
                {
                    request.steps.append(static_cast<RequestStep>(stepValue));
                } 
                else 
                {
    #ifdef TAO_DEBUG
                    // 处理非法值
                    qDebug() << "Invalid step value: " << stepValue << ", defaulting to Invalid";
    #endif
                    request.steps.append( RequestStep::Invalid);  // 设置为 Invalid 表示无效
                }
            }
            // 将请求添加到 UrlRequestManager
            QString requestId = requestManager->addRequest(request);
    #ifdef TAO_DEBUG
            qDebug() << "已存入Url缓冲区 ID: " << requestId;                // 保存生成的请求 ID
    #endif
            connectaddRequestId(requestId);
        }
    }
    else
    {
        isrepeatconnect = false;
    }
    saveUrlsToFile("RGA.json");
#ifdef TAO_DEBUG
    qDebug() << "所有URL数据已存入文件和缓冲区";
#endif

    return true;
}

void UrlGenerator::GenUrlDataresult(bool result)
{
#ifdef TAO_DEBUG
    qDebug() << "GenUrlDataresult is running: " << QThread::currentThreadId();
    qDebug() << "准备启动数据交互线程";
    qDebug() << "连接新增" << connectm_requestIds->size() << "个请求";
        // 打印当前请求总数
    qDebug() << "生成URL请求总数: " << m_connectuniqueUrls->size();

    requestManager->printAllRequests();        
    // 打印结束行
    qDebug() << "-----------------------------";

#endif

    emit dataUpdated();
}

void UrlGenerator::startscan(QString startMass,QString stopMass,QString ppamu,QString dwell,bool scanstate)
{   

    // 绑定工作任务和结果回调
    setStartMass(startMass);
    setStopMass(stopMass);
    setPpamu(ppamu);
    setDwell(dwell);
    setScanStatus(scanstate);
    // 绑定工作任务和结果回调
    WorkCallback workCall = std::bind(&UrlGenerator::ScanGenUrlDatawork, this); // 绑定成员函数 doWork
    WorkResultCallback resultCall = std::bind(&UrlGenerator::ScanGenUrlDataresult, this, std::placeholders::_1); // 绑定成员函数 handleResult

    // 使用线程池执行工作
    ThreadPool::getInstance()->work(workCall, resultCall);
}

void UrlGenerator::stopscan()
{
    requestManager->removeRequest(Scanm_requestIds);    
    Scanm_requestIds->clear();
    m_urlscanConfigstosave->clear();
    m_scanuniqueUrls->clear();

}

bool UrlGenerator::ScanGenUrlDatawork()
{
#ifdef TAO_DEBUG
    qDebug() << "扫描数据准备线程 is running: " << QThread::currentThreadId();
#endif

    addscanUrlConfig({"mmsp", "generalControl", "set"}, {{"setEmission", "On"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","first"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","6","set"}, 
    {{"channelMode","Baseline"},{"dwell","32"},{"ppamu","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","7","set"}, 
    {{"channelMode","TPQuad"},{"dwell","32"},{"ppamu","10"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","1","set"}, 
    {{"channelMode","FixedNumber"},{"dwell","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","2","set"}, 
    {{"channelMode","Timestamp"},{"dwell","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","3","set"}, 
    {{"channelMode","SystemStatus"},{"dwell","1"},{"enabled","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","4","set"}, 
    {{"channelMode","Sweep"},{"startMass",getStartMass()},{"stopMass",getStopMass()},{"dwell",getDwell()},{"ppamu",getPpamu()},{"enabled","true"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","5","set"}, 
    {{"channelMode","TotalPressure"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","8","set"}, 
    {{"channelMode","TotalPressureDetectorCurrent"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","9","set"}, 
    {{"channelMode","EmissionCurrent"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","10","set"}, 
    {{"channelMode","AnodePotential"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","11","set"}, 
    {{"channelMode","ElectronEnergy"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","12","set"}, 
    {{"channelMode","FocusPotential"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup", "channels","13","set"}, 
    {{"channelMode","FilamentCurrent"},{"dwell","1"},{"enable","True"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","second"}});
    addscanUrlConfig({"mmsp", "scanSetup","set"}, {{"startChannel","1"},{"stopChannel","13"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","third"}});
    addscanUrlConfig({"mmsp", "scanSetup","set"}, 
    {{"startChannel","1"},{"stopChannel","13"},{"scanCount","-1"},{"scanInterval","0"},{"scanStart","1"}},{{"isRunning", false},{"step","Step2"},{"responseFormat","fourth"}});
    addscanUrlConfig({"mmsp", "measurement", "binaryData","get"}, {{"@start","0"}},{{"isRunning", true},{"step","Step2"}});

    if(isrepeatscan == false)
    {
        // 遍历配置列表生成 URL 请求
        for (const auto& config : *m_urlscanConfigstosave)
        {

    #ifdef TAO_DEBUG
            qDebug() << "UrlRequest URL: " << config["path"].toString();
    #endif
            // 创建 UrlRequest 对象
            UrlRequest request;
            request.url = config["path"].toString();
            
            if (config.contains("isRunning")) 
            {
                request.isRunning = config["isRunning"].toBool();
            }
            if(config.contains("responseFormat"))
            {
                request.responseFormat = config["responseFormat"].toString();
            }

            if (config.contains("step")) 
            {
                int stepValue = config["step"].toInt();  // 获取 step 的整数值

                // 检查该值是否在合法的范围内（1 到 5），然后转换为枚举类型
                if (stepValue >= 1 && stepValue <= 5) 
                {
                    request.steps.append(static_cast<RequestStep>(stepValue));  // 将整数转换为 RequestStep 枚举
                } 
                else 
                {
#ifdef TAO_DEBUG
                    // 处理非法值
                    qDebug() << "Invalid step value: " << stepValue << ", defaulting to Invalid";
#endif
                    request.steps.append( RequestStep::Invalid);  // 设置为 Invalid 表示无效
                }
            }
            // 将请求添加到 UrlRequestManager
            QString requestId = requestManager->addRequest(request);
#ifdef TAO_DEBUG
            qDebug() << "已存入UrlRequestManager Request ID: " << requestId;            // 保存生成的请求 ID
#endif
            ScanaddRequestId(requestId);
        }
    }
    else
    {
        isrepeatscan = false;
    }

    return true;
}

void UrlGenerator::ScanGenUrlDataresult(bool result)
{
#ifdef TAO_DEBUG
    qDebug() << "ScanGenUrlDataresult is running: " << QThread::currentThreadId();
    qDebug() << "准备启动数据交互线程";

    qDebug() << "扫描新增" << Scanm_requestIds->size() << "个请求";
        // 打印当前请求总数
    qDebug() << "生成URL请求总数: " << m_scanuniqueUrls->size();
        // 打印结束行
    qDebug() << "-----------------------------";
    requestManager->printAllRequests();
#endif

    emit dataUpdatedscan(getStartMass(),getStopMass(),getPpamu(),getDwell());
}

// 子线程中添加请求ID
void UrlGenerator::connectaddRequestId(const QString& requestId) 
{
    QMutexLocker locker(&mutex);  // 使用锁保护共享数据
    connectm_requestIds->append(requestId);
}

void UrlGenerator::ScanaddRequestId(const QString& requestId) 
{
    QMutexLocker locker(&mutex);  // 使用锁保护共享数据
    Scanm_requestIds->append(requestId);
}

void UrlGenerator::addconnectUrlConfig(const QStringList& pathParts, const QList<QPair<QString, QVariant>>& params, const QList<QPair<QString, QVariant>>& paramsForBuffer)
{
    // 构建 URL
    QString url = generateUrl(pathParts, params);

    // 将参数存储到 QMap<QString, QString> 中
    QMap<QString, QString> paramMap;
    for (const auto& param : params) {
        paramMap.insert(param.first, param.second.toString());
    }

    // 获取一级目录 (去掉 mmsp 作为根)
    QString category = pathParts.isEmpty() ? "" : pathParts[1];

    // 如果没有重复的 URL，则添加
    if (!m_connectuniqueUrls->contains(url)) 
    {
        m_connectuniqueUrls->insert(url);

        // 创建新的 URL 配置项
        QMap<QString, QVariant> config;
        config.insert("category", category);
        config.insert("path", url);
        config.insert("params", QVariant::fromValue(paramMap));
        config.insert("method", "GET");  // 默认请求方法

        // 添加到 URL 配置列表
        m_urlConfigs->append(config);

        QMap<QString, QVariant> configToSave;
        configToSave.insert("category", category);
        configToSave.insert("path", url);
        configToSave.insert("params", QVariant::fromValue(paramMap));
        configToSave.insert("method", "GET");

        for (const auto& param : paramsForBuffer) 
        {
            if (param.first == "isRunning") {
                configToSave.insert("isRunning", param.second.toBool());
            }
            else if (param.first == "step") {
                // 获取 step 字符串
                QString stepString = param.second.toString();
                
                // 将 step 字符串转换为枚举值
                RequestStep step = requestManager->stringToStep(stepString);  // 转换为枚举值

                // 将枚举值存入 configToSave，若需要存储为整数
                configToSave.insert("step", static_cast<int>(step));  // 转换为整数存储
                
                // 如果需要存储为字符串，也可以直接存储
                // configToSave.insert("step", stepString);
            }
            else if(param.first == "responseFormat")
            {
                configToSave.insert("responseFormat", param.second.toString());
            }
        }
        // 添加到 URL 配置列表
        m_urlconnectConfigstosave->append(configToSave);
    } 
    else 
    {
        qDebug() << "重复 URL: " << url;
        isrepeatconnect = true;
    }
}
void UrlGenerator::addscanUrlConfig(const QStringList& pathParts, const QList<QPair<QString, QVariant>>& params, const QList<QPair<QString, QVariant>>& paramsForBuffer)
{
    // 构建 URL
    QString url = generateUrl(pathParts, params);

    // 将参数存储到 QMap<QString, QString> 中
    QMap<QString, QString> paramMap;
    for (const auto& param : params) {
        paramMap.insert(param.first, param.second.toString());
    }

    // 获取一级目录 (去掉 mmsp 作为根)
    QString category = pathParts.isEmpty() ? "" : pathParts[1];

    // 如果没有重复的 URL，则添加
    if (!m_scanuniqueUrls->contains(url)) 
    {
        m_scanuniqueUrls->insert(url);

        // 创建新的 URL 配置项
        QMap<QString, QVariant> config;
        config.insert("category", category);
        config.insert("path", url);
        config.insert("params", QVariant::fromValue(paramMap));
        config.insert("method", "GET");  // 默认请求方法

        // 添加到 URL 配置列表
        m_urlConfigs->append(config);

        QMap<QString, QVariant> configToSave;
        configToSave.insert("category", category);
        configToSave.insert("path", url);
        configToSave.insert("params", QVariant::fromValue(paramMap));
        configToSave.insert("method", "GET");


        for (const auto& param : paramsForBuffer) 
        {
            if (param.first == "isRunning") {
                configToSave.insert("isRunning", param.second.toBool());
            }
            else if (param.first == "step") {
                // 获取 step 字符串
                QString stepString = param.second.toString();
                
                // 将 step 字符串转换为枚举值
                RequestStep step = requestManager->stringToStep(stepString);  // 转换为枚举值

                // 将枚举值存入 configToSave，若需要存储为整数
                configToSave.insert("step", static_cast<int>(step));  // 转换为整数存储
                
                // 如果需要存储为字符串，也可以直接存储
                // configToSave.insert("step", stepString);
            }
            else if(param.first == "responseFormat")
            {
                configToSave.insert("responseFormat", param.second.toString());
            }
        }
        // 添加到 URL 配置列表
        m_urlscanConfigstosave->append(configToSave);
    } 
    else 
    {
        qDebug() << "重复 URL: " << url;
        isrepeatscan = true;
    }
}
// 生成 URL
QString UrlGenerator::generateUrl(const QStringList& pathParts, const QList<QPair<QString, QVariant>>& params) 
{
    QString baseUrl = QString("http://%1:%2").arg(getRGAip()).arg(getRGAport());  // 使用宏来构建基础 URL
    
    QString path = pathParts.join("/");

    // 如果路径没有包含协议，就加上基础 URL
    if (!path.startsWith("http://")) {
        path = baseUrl + "/" + path;
    }

    // 生成查询参数
    QStringList queryParams;
    for (const auto& param : params) {
        // 如果参数没有值，跳过它
        if (!param.second.isNull() && !param.second.toString().isEmpty()) {
            queryParams.append(QString("%1=%2").arg(param.first).arg(param.second.toString()));
        } else {
            queryParams.append(param.first);  // 没有值时只加参数名
        }
    }

    // 拼接完整的 URL
    if (!queryParams.isEmpty()) {
        return path + "?" + queryParams.join("&");
    } else {
        return path;
    }
}

void UrlGenerator::saveUrlsToFile(const QString& fileName)
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString urlsDir = appDir + "/urls";  // 设置 URLs 文件存储目录
    QDir dir(urlsDir);
    if (!dir.exists()) {
        dir.mkpath(urlsDir);
    }

    QString filePath = urlsDir + "/" + fileName;

    QMap<QString, QList<UrlEntry>> groupedUrls;

    // 将每个 URL 配置项转换为 UrlEntry 并按 category 分组
    for (const auto& config : *m_urlConfigs) {
        UrlEntry entry;
        entry.category = config["category"].toString();
        entry.path = config["path"].toString();
        entry.timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);  // 保存本地时间
        entry.method = "GET";  // 请求方法可以扩展

        // 将 URL 请求分组
        groupedUrls[entry.category].append(entry);
    }

    // 保存到文件
    QJsonObject jsonObj;
    for (auto category : groupedUrls.keys()) {
        QJsonArray categoryArray;
        for (const auto& entry : groupedUrls[category]) {
            QJsonObject entryObj;
            entryObj["path"] = entry.path;
            entryObj["timestamp"] = entry.timestamp;
            entryObj["method"] = entry.method;
            categoryArray.append(entryObj);
        }
        jsonObj[category] = categoryArray;
    }

    // 写入文件
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(jsonObj);
        file.write(doc.toJson());
    }
}

QList<UrlRequest> UrlGenerator::getRequestsByFilter(const RequestFilter& filter)
{
    return requestManager->getRequestsByFilter(filter);
}

void UrlGenerator::updateRequestsByFilter(const RequestFilter& filter, 
                                 const std::optional<bool>& newIsRunning, 
                                 const std::optional<QList<RequestStep>>& newSteps,
                                 const std::optional<QString>& newResponseFormat,
                                 const std::optional<int>& newPriority)
{
    requestManager->updateRequestsByFilter(filter, newIsRunning, newSteps,newResponseFormat,newPriority);
}

void UrlGenerator::updateRequest(const UrlRequest& request,
                    const std::optional<bool>& newIsRunning,
                    const std::optional<QList<RequestStep>>& newSteps,
                    const std::optional<QString>& newResponseFormat,
                    const std::optional<int>& newPriority)
{
    requestManager->updateRequest(request, newIsRunning, newSteps, newResponseFormat,newPriority);
}

bool UrlGenerator::findUrlInRequests(const QList<UrlRequest>& requests, const QString& urlPart)
{
    return requestManager->findUrlInRequests(requests, urlPart);
}


QList<UrlRequest> UrlGenerator::findRequestsByIds(const QList<QString>& requestIds)
{
    return requestManager->findRequestsByIds(requestIds);
}

void UrlGenerator::removeRequest(const UrlRequest& request)
{
    requestManager->removeRequest(request);
}
// 新增：获取所有请求
QList<UrlRequest> UrlGenerator::getAllRequests() 
{
    return requestManager->getAllRequests();
}

// 新增：根据请求 ID 获取请求
UrlRequest UrlGenerator::getRequestById(const QString& requestId) 
{
    return requestManager->findRequest(requestId);
}

// 新增：删除请求
void UrlGenerator::deleteRequest(const QString& requestId) 
{
    requestManager->removeRequest(requestId);
}

// 新增：清空所有请求
void UrlGenerator::clearAllRequests() 
{
    requestManager->removeAllRequests();
    
}

// 获取所有生成的请求 ID
QList<QString> UrlGenerator::getconnectAllRequestIds() 
{
    QMutexLocker locker(&mutex);  // 使用锁保护共享数据
    return *connectm_requestIds;
}

QString UrlGenerator::stepToString(RequestStep step)
{
    return requestManager->stepToString(step);
}

void UrlGenerator::setRGAip(const QString& ip)
{
    QMutexLocker locker(&mutex);
    _pRGAip = ip;
}
void UrlGenerator::setRGAport(int port)
{
    QMutexLocker locker(&mutex);
    _pRGAport = port;
}

QString UrlGenerator::getRGAip()
{
    QMutexLocker locker(&mutex);
    return _pRGAip;
}
int UrlGenerator::getRGAport()
{
    QMutexLocker locker(&mutex);
    return _pRGAport;
}

void UrlGenerator::setConnectStatus(bool checked)
{
    QMutexLocker locker(&mutex);
    isConnect = checked;
}

void UrlGenerator::setScanStatus(bool checked)
{
    QMutexLocker locker(&mutex);
    isScan = checked;
}
void UrlGenerator::UrlGenerator::setConnectDeviceStatus(bool checked)
{
    QMutexLocker locker(&mutex);
    isConnectDevice = checked;
}

bool UrlGenerator::getConnectStatus()
{
    QMutexLocker locker(&mutex);
    return isConnect;
}
bool UrlGenerator::getScanStatus()
{
    QMutexLocker locker(&mutex);
    return isScan;
}
bool UrlGenerator::getConnectDeviceStatus()
{
    QMutexLocker locker(&mutex);
    return isConnectDevice;
}
void UrlGenerator::setStartMass(const QString& startMass)
{
    QMutexLocker locker(&mutex);
    m_startMass = startMass;
}
void UrlGenerator::setStopMass(const QString& stopMass)
{
    QMutexLocker locker(&mutex);
    m_stopMass = stopMass;
}

void UrlGenerator::setPpamu(const QString& ppamu)
{
    QMutexLocker locker(&mutex);
    m_ppamu = ppamu;
}
void UrlGenerator::setDwell(const QString& dwell)
{
    QMutexLocker locker(&mutex);
    m_dwell = dwell;
}

QString UrlGenerator::getStartMass()
{
    QMutexLocker locker(&mutex);
    return m_startMass;
}
QString UrlGenerator::getStopMass()
{
    QMutexLocker locker(&mutex);
    return m_stopMass;
}
QString UrlGenerator::getPpamu()
{
    QMutexLocker locker(&mutex);
    return m_ppamu;
}

QString UrlGenerator::getDwell()
{
    QMutexLocker locker(&mutex);
    return m_dwell;
}
