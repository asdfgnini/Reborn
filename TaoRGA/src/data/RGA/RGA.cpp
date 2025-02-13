#include "RGA.h"
#include "DeviceMonitor.h"
#include "DeviceStateManager.h"
#include "ScanSetupChannel.h"
#include "URLManger.h"
#include "UrlGenerator.h"
#include <qnamespace.h>
#include <qthread.h>


RGA::RGA(QObject* parent) : QObject(parent)
, m_urlGenerator(new UrlGenerator())
, m_deviceMonitor(new DeviceMonitor(parent,m_urlGenerator))
,m_dataFetcher(new DataFetcher(parent))
,m_scanstate(false)
,m_currentscanstate_device(false)
,m_lastscanstate_device(false)

{
    // 连接数据更新信号
    connect(m_urlGenerator,&UrlGenerator::dataUpdated,this,&RGA::ondataUpdated);

    // 连接停止连接信号
    connect(m_deviceMonitor,&DeviceMonitor::stopconnectSignal,this,&RGA::onstopconnect);

    // 连接数据扫描更新信号
    connect(m_urlGenerator, &UrlGenerator::dataScanUpdated, this, &RGA::onDataScanUpdated);

    // 连接停止扫描信号
    connect(m_dataFetcher,&DataFetcher::stopscanSignal,this,&RGA::onstopscanSignal);

    // 连接数据接收信号
    connect(m_dataFetcher,&DataFetcher::newDataAvailable,this,&RGA::dataReceived);
}
 
RGA::~RGA()
{
    m_dataFetcher->stopscan(false);
    stopconnect(false);    
    delete m_urlGenerator;

}


// 给外界调用，启用URL生成线程,主要响应 连接 按钮的信号
void RGA::startconnect(QString ip, int port,bool checked)
{
    m_ip = ip;
    m_port = port;
    
    //先生成所需要的URL
    m_urlGenerator->startConnect(ip, port, checked);
}

//给外界调用，停止连接
void RGA::stopconnect(bool isscan)
{
    if(isscan)//关闭连接时需要不需要停止获取数据，设备的扫描没有关
    {
        m_scanstate = false;
        //停止扫描,不需要发送信号
        m_dataFetcher->stopscan(true);

        //停止连接
        m_deviceMonitor->stopconnect();


    }
    else
    {
        m_deviceMonitor->stopconnect();
    }
}
// 给外界调用，启用URL生成线程,主要响应 扫描 按钮的信号
void RGA::startscan(QString startMass,QString stopMass,QString ppamu,QString dwell,bool scanstate)
{
    m_startMass = startMass.toInt();
    m_stopMass = stopMass.toInt();
    m_ppamu = ppamu.toInt();
    m_scanstate = scanstate;
    //将urlGenerator赋值给dataFetcher，方便后续从缓冲区获取数据
    m_dataFetcher->setm_urlGenerator(m_urlGenerator);

    //准备此次事件所需要的URL    
    m_urlGenerator->startScan(startMass, stopMass, ppamu, dwell, scanstate);
}
//给外界调用，停止扫描
void RGA::stopscan(bool scanstate)
{
    m_scanstate = scanstate;


    QString url = QString("http://%1:%2/mmsp/communication/control/set?force").arg(m_ip).arg(m_port);
    QString retStr;
    QString err;

    if(HttpRequestSync(url, retStr, err, 2000, ResponseFormat::JSON))
    {
        qDebug() << "开始申请强制控制123 " << retStr;
        bool ok;
        QJsonObject jsonObj = m_deviceMonitor->parseJsonString(retStr, &ok);
        if (!ok) {
            qDebug() << "Failed to parse JSON response";
            return;
        }
        QString name = jsonObj["name"].toString();
        if (name != "wasSet") {
            qDebug() << "----------------------------------------------------------------";
            qDebug() << "申请强制控制失败: name is not 'wasSet', got:" << name;
            qDebug() << "----------------------------------------------------------------";
            return;
        }
        else
        {
            qDebug() << "----------------------------------------------------------------";
            qDebug() << "申请强制控制成功";
            qDebug() << "----------------------------------------------------------------";
        }
    }
    else //超时处理
    {
        qDebug() << "----------------------------------------------------------------";
        qDebug() << "申请强制控制失败";
        qDebug() << "----------------------------------------------------------------";
    }

    RequestFilter filter;
    filter.steps = {RequestStep::Step4};
    filter.urlPart = "scanSetup/set";
    filter.isRunning = false;

    auto requests = m_urlGenerator->getRequestsByFilter(filter);
    qDebug() << "stopscan requests size" << requests.size();

    for(const auto& request : requests)
    {
        QString retStr;
        QString err;
        qDebug() << "stopscan request url" << request.url;
        if(HttpRequestSync(request.url, retStr, err, 2000, ResponseFormat::JSON))
        {
            qDebug() << "stopscan retStr" << retStr;
            bool ok;
            QJsonObject jsonObj = m_deviceMonitor->parseJsonString(retStr, &ok);
            if(ok)
            {
               QString name = jsonObj["name"].toString();
               if(name != "wasSet")
               {
                    qDebug() << "stopscan request url" << request.url << "failed";
               }
               else
               {
                    qDebug() << "stopscan request url" << request.url << "success";
               }
            }
        }
        else//处理超时情况
        {
            qDebug() << "stopscan request url" << request.url << "failed";
        }
    }
}
/*
--------------------------------
    连接相关函数
--------------------------------
*/
//开始连接相关函数
//当url准备好了后，执行一次请求，获取基本设备信息
void RGA::ondataUpdated()
{
    auto requests = m_urlGenerator->findRequestsByUrlPart("controlInfo/get");
    if(requests.isEmpty())
    {
        qDebug() << "没有找到controlInfo/get的请求";
    }
    else
    {
        qDebug() << "找到controlInfo/get的请求" << requests[0].url;
        QString retStr;
        QString err;
        if(HttpRequestSync(requests[0].url, retStr, err, 2000, ResponseFormat::JSON))
        {
            // qDebug() << "controlInfo/get的请求成功" << retStr;
            bool ok;
            QJsonObject jsonObj = m_deviceMonitor->parseJsonString(retStr, &ok);
            if(ok)
            {
                m_currentcontrolinfo.amInControl = m_deviceMonitor->getJsonValue(jsonObj, "data.amInControl").toBool();
                m_currentcontrolinfo.canForceControl = m_deviceMonitor->getJsonValue(jsonObj, "data.canForceControl").toBool();
                m_currentcontrolinfo.controlled = m_deviceMonitor->getJsonValue(jsonObj, "data.controlled").toBool();
                m_currentcontrolinfo.locked = m_deviceMonitor->getJsonValue(jsonObj, "data.locked").toBool();
                m_currentcontrolinfo.secondsSinceLastControlRequest = m_deviceMonitor->getJsonValue(jsonObj, "data.secondsSinceLastControlRequest").toUInt();
                m_currentcontrolinfo.secondsSinceLastDataRequest = m_deviceMonitor->getJsonValue(jsonObj, "data.secondsSinceLastDataRequest").toUInt();
                m_currentcontrolinfo.secondsSinceLastRequest = m_deviceMonitor->getJsonValue(jsonObj, "data.secondsSinceLastRequest").toUInt();

                m_lastcontrolinfo = m_currentcontrolinfo;
                qDebug() << "----------------------------------------------------------------";
                qDebug() << "更新控制信息完成";
                qDebug() << "----------------------------------------------------------------";

            }
        }
        else
        {
            qDebug() << "----------------------------------------------------------------";
            qDebug() << "controlInfo/get的请求失败" << err;
            qDebug() << "----------------------------------------------------------------";
        }

    }

    //获取设备基本信息,主要是获取startChannel,stopChannel,scanCount,scanInterval,leakCheckMass,scanTimeTotal,captureFileStatus
    RequestFilter filter;
    filter.steps = {RequestStep::Step1};
    connect(this,&RGA::deviceBaseInfoReady,this,&RGA::ondeviceBaseInfoReady);   

    executeUrlRequestsonce(filter, [this](const QString& response) {
        //解析设备基本信息,更新startChannel,stopChannel,scanCount,scanInterval,leakCheckMass,scanTimeTotal,captureFileStatus
        parseDeviceBaseInfo(response);
    });
}
//当设备基本信息获取完成时，会触发这个信号,获取当前设备的通道配置
void RGA::ondeviceBaseInfoReady()
{
    //更新设备基本信息
    DeviceStateManager::instance()->updateDeviceBaseInfo(DEVICE_ID0, m_deviceBaseInfo);
    qDebug() << "----------------------------------------------------------------";
    qDebug() << "更新设备基本信息完成";
    qDebug() << "----------------------------------------------------------------";
    //打印刚刚获取的设备基本信息
    DeviceBaseInfo deviceBaseInfo = DeviceStateManager::instance()->getDeviceBaseInfo(DEVICE_ID0);

    // 批量生成所有通道的请求
    QVector<std::pair<RequestFilter, int>> requests;
    for(int i = 1; i <= m_deviceBaseInfo.stopChannel; i++)
    {

        RequestFilter filter;
        filter.steps = {RequestStep::Step2};
        filter.priority = i;
        requests.push_back({filter, i});
    }

    // 连接信号，处理所有通道信息获取完成的情况
    connect(this, &RGA::allChannelsInfoReady, this, &RGA::onAllChannelsInfoReady);

    // 执行批量请求
    executeUrlRequestsBatch(requests, [this](const QString& response, int channelId) {
        parseChannelInfo(response,channelId);
    });
}
//当所有通道配置获取完成时，获取设备扫描状态，并开启设备状态监控
void RGA::onAllChannelsInfoReady()
{
    //打印所有通道配置
    ScanSetupChannel::instance()->updateChannels(SCAN_DEVICE, m_channelConfigs);
    qDebug() << "----------------------------------------------------------------";
    qDebug() << "设备通道更新完成";
    qDebug() << "----------------------------------------------------------------";
    //打印所有通道配置
    // ScanSetupChannel::instance()->printAllChannels(SCAN_DEVICE);

    // QString url = QString("http://%1:%2/mmsp/communication/control/set?force").arg(m_ip).arg(m_port);
    QString retStr;
    QString err;

    // if(HttpRequestSync(url, retStr, err, 2000, ResponseFormat::JSON))
    // {
    //     qDebug() << "开始申请强制控制 " << retStr;
    //     bool ok;
    //     QJsonObject jsonObj = m_deviceMonitor->parseJsonString(retStr, &ok);
    //     if (!ok) {
    //         qDebug() << "Failed to parse JSON response";
    //         return;
    //     }
    //     QString name = jsonObj["name"].toString();
    //     if (name != "wasSet") {
    //         qDebug() << "----------------------------------------------------------------";
    //         qDebug() << "申请强制控制失败: name is not 'wasSet', got:" << name;
    //         qDebug() << "----------------------------------------------------------------";
    //         return;
    //     }

    //     else
    //     {
    //         qDebug() << "----------------------------------------------------------------";
    //         qDebug() << "申请强制控制成功";
    //         qDebug() << "----------------------------------------------------------------";
    //     }
    // }
    // else //超时处理
    // {
    //     qDebug() << "----------------------------------------------------------------";
    //     qDebug() << "申请强制控制失败";
    //     qDebug() << "----------------------------------------------------------------";
    // }

    // http://192.168.1.101:80/mmsp/scanInfo/get
    auto requests = m_urlGenerator->findRequestsByUrlPart("scanInfo/get");
    if(requests.isEmpty())
    {
        qDebug() << "没有找到scanInfo/get的请求";
    }
    else
    {   
        qDebug() << "找到scanInfo/get的请求" << requests[0].url;
        //获取设备是否已经开始扫描
        if(HttpRequestSync(requests[0].url, retStr, err, 2000, ResponseFormat::JSON))
        {
            //解析设备是否已经开始扫描
            bool ok;    
            QJsonObject jsonObj = m_deviceMonitor->parseJsonString(retStr, &ok);
            if(ok)
            {
                bool isScanning = m_deviceMonitor->getJsonValue(jsonObj, "data.scanning").toBool();
                m_currentscanstate_device = isScanning;
                m_lastscanstate_device = m_currentscanstate_device;
                qDebug() << "设备是否已经开始扫描" << isScanning;
                if(isScanning)
                {
                    //如果设备已经开始扫描，则需要同步数据
                    m_dataFetcher->setm_urlGenerator(m_urlGenerator);
                    m_dataFetcher->setparseChannelType(SCAN_DEVICE);
                    m_scanstate = true;
                    m_dataFetcher->startscan();

                    emit connectisready(isScanning,deviceStartMass,deviceStopMass,devicePpamu,deviceDwell);
                }
                else
                {
                    emit connectisready(isScanning,0,0,0,0);
                }
            }
        }
    }

    qDebug() << "准备开启设备状态监控";
    //开启设备状态监控，开启后，会自动获取状态，并更新到DeviceStateManager中
    m_deviceMonitor->startconnect();
    //设置完成后,由监控线程去获取状态跟新状态，我们绑定状态改变的信号，当状态改变且满足条件时
    connect(DeviceStateManager::instance(),&DeviceStateManager::scanStatusChanged,this,&RGA::onscanStatusChanged);

    // //关注控制信息
    connect(DeviceStateManager::instance(),&DeviceStateManager::controlInfoChanged,this,&RGA::oncontrolInfoChanged);

    //关注测量状态
    connect(DeviceStateManager::instance(),&DeviceStateManager::measurementStatusChanged,this,&RGA::onmeasurementStatusChanged);
}
//停止连接相关函数
// 停止连接后，发送信号，表示连接已关闭，更改前端UI
void RGA::onstopconnect()
{
    disconnect(this, &RGA::deviceBaseInfoReady, this, &RGA::ondeviceBaseInfoReady);
    disconnect(this, &RGA::allChannelsInfoReady, this, &RGA::onAllChannelsInfoReady);

        //设置完成后,由监控线程去获取状态跟新状态，我们绑定状态改变的信号，当状态改变且满足条件时
    disconnect(DeviceStateManager::instance(),&DeviceStateManager::scanStatusChanged,this,&RGA::onscanStatusChanged);

    // //关注控制信息
    disconnect(DeviceStateManager::instance(),&DeviceStateManager::controlInfoChanged,this,&RGA::oncontrolInfoChanged);

    //关注测量状态
    disconnect(DeviceStateManager::instance(),&DeviceStateManager::measurementStatusChanged,this,&RGA::onmeasurementStatusChanged);

    m_urlGenerator->stopConnect();

    emit stopconnectisready();    
}
/*
--------------------------------
    扫描相关函数
--------------------------------
*/
//开始扫描相关函数
//扫描操作，当用户点击扫描按钮时，会触发这个信号，用于设置SCAN_USER的通道配置到设备，获取m_startMass,m_stopMass,m_ppamu,m_dwell
void RGA::onDataScanUpdated(QString startMass, QString stopMass, QString ppamu, QString dwell)
{
    //配置用户通道设置之前先申请强制控制
    QString url = QString("http://%1:%2/mmsp/communication/control/set?force").arg(m_ip).arg(m_port);
    QString retStr;
    QString err;

    if(HttpRequestSync(url, retStr, err, 2000, ResponseFormat::JSON))
    {
        qDebug() << "开始申请强制控制 " << retStr;
        bool ok;
        QJsonObject jsonObj = m_deviceMonitor->parseJsonString(retStr, &ok);
        if (!ok) {
            qDebug() << "Failed to parse JSON response";
            return;
        }
        QString name = jsonObj["name"].toString();
        if (name != "wasSet") {
            qDebug() << "----------------------------------------------------------------";
            qDebug() << "申请强制控制失败: name is not 'wasSet', got:" << name;
            qDebug() << "----------------------------------------------------------------";
            return;
        }
        else
        {
            qDebug() << "----------------------------------------------------------------";
            qDebug() << "申请强制控制成功";
            qDebug() << "----------------------------------------------------------------";
        }
    }
    else //超时处理
    {
        qDebug() << "----------------------------------------------------------------";
        qDebug() << "申请强制控制失败";
        qDebug() << "----------------------------------------------------------------";
    }

    //生成和注册用户通道配置
    UserChannelConfig(m_startMass,m_stopMass,m_ppamu);

    connect(this,&RGA::channelsetisready,this,&RGA::onchannelsetisready);
    //先设置通道
    RequestFilter filter;
    filter.steps = {RequestStep::Step4};
    executeUrlRequestsoncesort(filter, [this](const QString& response) {

    });

}
//扫描操作，当用户通道设置完成时，会触发这个信号
void RGA::onchannelsetisready(QList<QString> failedUrls)
{
    if(!failedUrls.isEmpty())
    {
        qDebug() << "有通道设置失败" << failedUrls;

        m_scanstate = false;
        m_urlGenerator->stopScan();
        //移除channelsetisready的信号
        disconnect(this,&RGA::channelsetisready,this,&RGA::onchannelsetisready);

        emit scanisready(false);
        return;
    }

    //emit scanisready(true);
}

//停止扫描相关函数
//停止扫描，当用户点击停止扫描按钮时，会触发这个信号，用于停止扫描
void RGA::onstopscanSignal(bool iscan)
{
    m_urlGenerator->stopScan();
    
    //移除channelsetisready的信号
    disconnect(this,&RGA::channelsetisready,this,&RGA::onchannelsetisready);

    //发送信号，表示扫描已停止
    emit stopscanisready(iscan);    
}
/*
--------------------------------
    执行URL请求相关函数
--------------------------------
*/
//执行URL请求，支持单个请求
void RGA::executeUrlRequestsonce(const RequestFilter& filter, std::function<void(const QString&)> successCallback)
{
    WorkCallback workCall = [this, filter, successCallback]()->bool {

        QList<UrlRequest> requests = m_urlGenerator->getRequestsByFilter(filter);
        qDebug() << "requests size" << requests.size();

        QString retStr;
        QString err;
        bool hasSuccess = false;  // 记录是否有成功的请求

        for(const UrlRequest& request : requests)
        {
            // qDebug() << "request url:" << request.url;
            if(HttpRequestSync(request.url, retStr, err, 2000, ResponseFormat::JSON))
            {
                // qDebug() << "Request successful for url:" << request.url;
                // qDebug() <<"----------------------------------------------------------------";
                if(successCallback) {
                    successCallback(retStr);
                }
                hasSuccess = true;  // 标记有成功的请求
            }
            else
            {
                // qDebug() << "request failed for url:" << request.url << ", error:" << err;
                // qDebug() <<"----------------------------------------------------------------";
                // 不直接返回 false，继续处理其他请求
            }
        }

        // 只要有一个请求成功就返回 true
        return hasSuccess;
    };

    WorkResultCallback resultCall = [this,filter](bool isSuccess) {
        if(!isSuccess) {
            qDebug() << "All requests failed or no successful requests";
        }
        else
        {
            emit deviceBaseInfoReady();
        }
    };

    ThreadPool::getInstance()->work(workCall, resultCall);
}

//执行URL请求，支持批量请求
void RGA::executeUrlRequestsBatch(const QVector<std::pair<RequestFilter, int>>& requests, 
                                std::function<void(const QString&, int)> successCallback)
{
    WorkCallback workCall = [this, requests, successCallback]()->bool {
        bool hasSuccess = false;


        // 按顺序处理所有请求
        for(const auto& request : requests)
        {
            const auto& filter = request.first;
            int channelId = request.second;
            
            QString retStr;
            QString err;
            auto urlRequests = m_urlGenerator->getRequestsByFilter(filter);
            
            for(const auto& urlRequest : urlRequests)
            {
                // qDebug() << "Processing request for channel" << channelId 
                //         << "URL:" << urlRequest.url;
                
                if(HttpRequestSync(urlRequest.url, retStr, err, 2000, ResponseFormat::JSON))
                {
                    // qDebug() << "Request successful for channel" << channelId;
                    // qDebug() <<"----------------------------------------------------------------";
                    if(successCallback) {
                        successCallback(retStr, channelId);
                    }
                    hasSuccess = true;
                }
                else
                {
                    // qDebug() << "Request failed for channel" << channelId 
                    //         << "Error:" << err;
                    // qDebug() <<"----------------------------------------------------------------";
                }
            }
        }
        
        return hasSuccess;  // 只要有一个请求成功就返回true
    };

    WorkResultCallback resultCall = [this](bool success) {
        if(!success) {
            qDebug() << "All requests failed";  // 只有所有请求都失败才会显示此消息
        }
        emit allChannelsInfoReady();
    };

    ThreadPool::getInstance()->work(workCall, resultCall);
}
//执行URL请求，并按优先级排序
void RGA::executeUrlRequestsoncesort(const RequestFilter& filter, std::function<void(const QString&)> successCallback)
{
    WorkCallback workCall = [this, filter, successCallback]()->bool {
        QList<UrlRequest> requests = m_urlGenerator->getRequestsByFilter(filter);
        qDebug() << "Original requests size:" << requests.size();

        // 按priority排序
        std::sort(requests.begin(), requests.end(), 
            [](const UrlRequest& a, const UrlRequest& b) {
                return a.priority < b.priority;
            });

        QString retStr;
        QString err;
        bool hasSuccess = false;
        QList<QString> failedUrls;  // 用于记录失败的URL

        // 按排序后的顺序执行请求
        for(const UrlRequest& request : requests)
        {
            if(HttpRequestSync(request.url, retStr, err, 2000, ResponseFormat::JSON))
            {
                // 解析响应JSON
                bool ok;
                QJsonObject jsonObj = m_deviceMonitor->parseJsonString(retStr, &ok);
                if (!ok) {
                    qDebug() << "Failed to parse JSON response";
                    continue;
                }

                // 检查name字段
                QString name = jsonObj["name"].toString();
                if (name != "wasSet") {
                    qDebug() << "Request failed: name is not 'wasSet', got:" << name;
                    failedUrls.append(request.url);  // 记录失败的URL
                    continue;
                }

                qDebug() << "Request successful for url:" << request.url 
                        << "(Priority:" << request.priority << ")";
                qDebug() <<"----------------------------------------------------------------";
                
                if(successCallback) {
                    successCallback(retStr);
                }
                hasSuccess = true;
            }
            else
            {
                qDebug() << "Request failed for url:" << request.url 
                        << "(Priority:" << request.priority << ")"
                        << ", error:" << err;
                qDebug() <<"----------------------------------------------------------------";
                break; // 如果请求失败，终止后续请求
            }
            QThread::msleep(100);
        }

        // 发送失败的URL信息
        if (!failedUrls.isEmpty()) {
            emit channelsetisready(failedUrls);
        }

        return hasSuccess;
    };

    WorkResultCallback resultCall = [this,filter](bool isSuccess) {
        if(!isSuccess) {
            qDebug() << "All requests failed or no successful requests";
        }
        else
        {
            emit channelsetisready(QList<QString>());
        }
    };

    ThreadPool::getInstance()->work(workCall, resultCall);
}

//执行URL请求
bool RGA::HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, int timeout, ResponseFormat format)
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
        qDebug() << "请求超时！超时时间：" << timeout << "ms";
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
/*
--------------------------------
    解析设备基本信息相关函数
--------------------------------
*/
//解析设备基本信息
void RGA::parseDeviceBaseInfo(const QString& response)
{
#ifdef TAO_DEBUG

    // qDebug() << "parseDeviceBaseInfo" << response;
#endif
    bool ok;
    QJsonObject jsonObj = m_deviceMonitor->parseJsonString(response, &ok);
    if (!ok) {

        qDebug() << "解析JSON数据失败";
        return;
    }

    QString origin = m_deviceMonitor->getJsonValue(jsonObj, "origin").toString();
    if (origin.isEmpty()) {
        qDebug() << "获取origin字段失败";
        return;
    }
    //更新设备基本信息
    if(origin.contains("scanTimeTotal"))
    {
        //获取scanTimeTotal 
        uint32_t scanTimeTotal = m_deviceMonitor->getJsonValue(jsonObj, "data").toUInt();
        qDebug() << "scanTimeTotal" << scanTimeTotal;
        m_deviceBaseInfo.scanTimeTotal = scanTimeTotal;
    }
    else if(origin.contains("scanCount"))
    {
        //获取scanCount

        int16_t scanCount = m_deviceMonitor->getJsonValue(jsonObj, "data").toInt();
        qDebug() << "scanCount" << scanCount;
        m_deviceBaseInfo.scanCount = scanCount;

    }
    else if(origin.contains("scanInterval"))
    {
        //获取scanInterval
        uint32_t scanInterval = m_deviceMonitor->getJsonValue(jsonObj, "data").toUInt();
        qDebug() << "scanInterval" << scanInterval;
        m_deviceBaseInfo.scanInterval = scanInterval;
    }
    else if(origin.contains("leakCheckMass"))

    {
        //获取leakCheckMass
        uint16_t leakCheckMass = m_deviceMonitor->getJsonValue(jsonObj, "data").toUInt();
        qDebug() << "leakCheckMass" << leakCheckMass;
        m_deviceBaseInfo.leakCheckMass = leakCheckMass;
    }
    else if(origin.contains("captureFileStatus"))

    {
        // 修正：先获取QJsonValue，然后转换为QJsonObject
        QJsonValue dataValue = jsonObj["data"];
        if (dataValue.isObject()) {
            QJsonObject dataObj = dataValue.toObject();
            CaptureFileStatus status;
            status.name = dataObj["name"].toString();
            status.size = dataObj["size"].toString();
            m_deviceBaseInfo.captureFileStatus = status;

        }
    }
    else if(origin.contains("startChannel"))
    {
        //获取startChannel
        uint16_t startChannel = m_deviceMonitor->getJsonValue(jsonObj, "data").toUInt();
        qDebug() << "startChannel" << startChannel;
        m_deviceBaseInfo.startChannel = startChannel;


    }
    else if(origin.contains("stopChannel"))
    {
        //获取stopChannel
        uint16_t stopChannel = m_deviceMonitor->getJsonValue(jsonObj, "data").toUInt();
        qDebug() << "stopChannel" << stopChannel;
        m_deviceBaseInfo.stopChannel = stopChannel;
    }
    else {
        qDebug() << "获取设备基本信息失败,没有找到origin字段中包含的参数";
    }
    
    
}
//解析通道信息，用于设置SCAN_DEVICE的通道配置，获取deviceStartMass,deviceStopMass,devicePpamu,deviceDwell
void RGA::parseChannelInfo(const QString& response,int channelId)
{
#ifdef TAO_DEBUG
    // qDebug() << "parseChannelInfo" << response << "channelId:" << channelId;
#endif

    bool ok;
    QJsonObject jsonObj = m_deviceMonitor->parseJsonString(response, &ok);
    if (!ok) {
        qDebug() << "解析JSON数据失败";
        return;
    }

    // 获取data数组的第一个元素
    QJsonValue dataValue = jsonObj["data"];
    if (!dataValue.isArray()) {
        qDebug() << "data 不是数组类型";
        return;
    }

    QJsonArray dataArray = dataValue.toArray();
    if (dataArray.isEmpty()) {
        qDebug() << "data 数组为空";
        return;
    }

    QJsonObject channelData = dataArray.first().toObject();
    
    ChannelConfig channelConfig;
    channelConfig.id = channelId;
    
    // 解析通道模式
    QString channelModeStr = channelData["channelMode"].toString();
    channelConfig.channelMode = parseChannelMode(channelModeStr);
    
    // 解析通道类型
    QString channelTypeStr = channelData["channelType"].toString();
    channelConfig.channelType = (channelTypeStr == "U32Value") ? 
        RGAType::ChannelType::U32Value : RGAType::ChannelType::F32Value;
    
    // 解析其他数值
    channelConfig.startMass = channelData["startMass"].toDouble();
    channelConfig.stopMass = channelData["stopMass"].toDouble();
    channelConfig.ppamu = channelData["ppamu"].toDouble();
    channelConfig.enabled = channelData["enabled"].toString().toLower() == "true";

    // 如果是扫描模式，更新设备参数
    if (channelConfig.channelMode == RGAType::ChannelMode::Sweep) {
        deviceStartMass = channelConfig.startMass;
        deviceStopMass = channelConfig.stopMass;
        devicePpamu = channelConfig.ppamu;
        deviceDwell = channelData["dwell"].toInt();
    }

    m_channelConfigs.append(channelConfig);
}
// 添加一个辅助函数来解析通道模式
RGAType::ChannelMode RGA::parseChannelMode(const QString& modeStr)
{
    static const QMap<QString, RGAType::ChannelMode> modeMap = {
        {"FixedNumber", RGAType::ChannelMode::FixedNumber},
        {"Timestamp", RGAType::ChannelMode::Timestamp},
        {"SystemStatus", RGAType::ChannelMode::SystemStatus},
        {"Sweep", RGAType::ChannelMode::Sweep},
        {"TotalPressure", RGAType::ChannelMode::TotalPressure},
        {"Baseline", RGAType::ChannelMode::Baseline},
        {"TPQuad", RGAType::ChannelMode::TPQuad},
        {"TotalPressureDetectorCurrent", RGAType::ChannelMode::TotalPressureDetectorCurrent},
        {"EmissionCurrent", RGAType::ChannelMode::EmissionCurrent},
        {"AnodePotential", RGAType::ChannelMode::AnodePotential},
        {"ElectronEnergy", RGAType::ChannelMode::ElectronEnergy},
        {"FocusPotential", RGAType::ChannelMode::FocusPotential},
        {"FilamentCurrent", RGAType::ChannelMode::FilamentCurrent},
        {"Pressure", RGAType::ChannelMode::Pressure},
        {"Time", RGAType::ChannelMode::Time},
        {"Single", RGAType::ChannelMode::Single}
    };

    return modeMap.value(modeStr, RGAType::ChannelMode::FixedNumber); // 默认返回 FixedNumber
}
/*
--------------------------------
    监控设备状态相关函数
--------------------------------
*/
//监控扫描状态，当设备扫描状态发生变化时，会触发这个信号
//监控扫描状态
void RGA::onscanStatusChanged(const QString& deviceId, const ScanStatus& status)
{
    //更新设备扫描状态
    m_currentscanstate_device = status.scanning;  
    //如果设备扫描状态与上次一致，则不处理
    if(m_currentscanstate_device == m_lastscanstate_device)
    {
        return;
    }
    else //设备扫描状态发生了改变
    {
        //更新上次设备扫描状态
        m_lastscanstate_device = m_currentscanstate_device;
        //这里说明在第一次获取状态时，设备没有开启扫描，但是现在设备状态发生了改变
        if(m_currentscanstate_device == true)
        {
            // //其他用户开启了扫描，我需要同步
            // if(m_scanstate == false)
            // {
            //     qDebug() << "----------------------------------------------------------------";
            //     qDebug() << "用户没有开启扫描，进入设备同步扫描状态,这里需要重新获取通道设置";
            //     qDebug() << "----------------------------------------------------------------";
            //     m_dataFetcher->setparseChannelType(SCAN_DEVICE);
            //     m_dataFetcher->startscan();
            // }
            // else //用户开启了扫描
            {
                qDebug() << "----------------------------------------------------------------";
                qDebug() << "用户开启了扫描，设备也开始扫描";
                qDebug() << "----------------------------------------------------------------";
                m_dataFetcher->setparseChannelType(SCAN_USER);
                m_dataFetcher->startscan();
                emit scanisready(true);
            }
        }
        else //设备关闭扫描
        {
            if(m_scanstate == false)
            {
                qDebug() << "----------------------------------------------------------------";
                qDebug() << "用户关闭扫描";
                qDebug() << "----------------------------------------------------------------";
                m_scanstate = false;
                // 停止扫描
                m_dataFetcher->stopscan(true);
            }
            else //其他用户关闭了扫描
            {
                qDebug() << "----------------------------------------------------------------";
                qDebug() << "其他用户关闭了扫描";
                qDebug() << "----------------------------------------------------------------";
                m_scanstate = false;
                // 停止扫描
                m_dataFetcher->stopscan(false);
            }
        }
    }
}
//监控控制信息
void RGA::oncontrolInfoChanged(const QString& deviceId, const ControlInfo& info)
{
    //先更新控制信息
    m_currentcontrolinfo = info;
    //如果当前控制信息与上次控制信息不一致，则说明控制信息发生了变化
    if(m_currentcontrolinfo.amInControl != m_lastcontrolinfo.amInControl ||
       m_currentcontrolinfo.canForceControl != m_lastcontrolinfo.canForceControl ||
       m_currentcontrolinfo.controlled != m_lastcontrolinfo.controlled ||
       m_currentcontrolinfo.locked != m_lastcontrolinfo.locked)
    {
        qDebug() << "控制信息发生变化";
        m_lastcontrolinfo = m_currentcontrolinfo;

        //如果我没有控制，并且没有锁定，并且被控制了，则可以强制控制
        if(m_currentcontrolinfo.amInControl == false && m_currentcontrolinfo.locked == false && m_currentcontrolinfo.controlled == true)
        {
            if(m_currentcontrolinfo.canForceControl == true)
            {
                QString url = QString("http://%1:%2/mmsp/communication/control/set?force").arg(m_ip).arg(m_port);
                QString retStr;
                QString err;

                if(HttpRequestSync(url, retStr, err, 2000, ResponseFormat::JSON))
                {
                    qDebug() << "可以强制控制 " << retStr;
                    bool ok;
                    QJsonObject jsonObj = m_deviceMonitor->parseJsonString(retStr, &ok);
                    if (!ok) {
                        qDebug() << "Failed to parse JSON response";
                        return;
                    }
                    QString name = jsonObj["name"].toString();
                    if (name != "wasSet") {
                        qDebug() << "申请强制控制失败: name is not 'wasSet', got:" << name;
                        return;
                    }

                    else
                    {
                        qDebug() << "申请强制控制成功";
                    }

                }
                else //超时处理
                {
                    qDebug() << "申请强制控制失败";
                }

            }
        }
    }
}

/*
--------------------------------
    用户通道设置相关函数
--------------------------------
*/
//生成和注册用户通道设置
void RGA::UserChannelConfig(int startMass,int stopMass,int ppamu)
{
    QVector<ChannelConfig> configs;
    ChannelConfig config1;
    config1.id = 1;
    config1.enabled = true;
    config1.channelMode = RGAType::ChannelMode::FixedNumber;
    config1.startMass = 0;
    config1.stopMass = 0;
    config1.ppamu = 10;
    config1.channelType = RGAType::ChannelType::U32Value;
    configs.append(config1);
    ChannelConfig config2;
    config2.id = 2;
    config2.enabled = true;
    config2.channelMode = RGAType::ChannelMode::Timestamp;
    config2.startMass = 0;
    config2.stopMass = 0;
    config2.ppamu = 10;
    config2.channelType = RGAType::ChannelType::U32Value;
    configs.append(config2);
    ChannelConfig config3;
    config3.id = 3;
    config3.enabled = true;
    config3.channelMode = RGAType::ChannelMode::SystemStatus;
    config3.startMass = 0;
    config3.stopMass = 0;
    config3.ppamu = 10;
    config3.channelType = RGAType::ChannelType::U32Value;
    configs.append(config3);
    ChannelConfig config4;
    config4.id = 4;
    config4.enabled = true;
    config4.channelMode = RGAType::ChannelMode::Sweep;
    config4.startMass = m_startMass;
    config4.stopMass = m_stopMass;
    config4.ppamu = m_ppamu;
    config4.channelType = RGAType::ChannelType::U32Value;
    configs.append(config4);
    ChannelConfig config5;
    config5.id = 5;
    config5.enabled = true;
    config5.channelMode = RGAType::ChannelMode::TotalPressure;
    config5.startMass = 0;
    config5.stopMass = 0;
    config5.ppamu = 10;
    config5.channelType = RGAType::ChannelType::F32Value;
    configs.append(config5);
    ChannelConfig config6;
    config6.id = 6;
    config6.enabled = true;
    config6.channelMode = RGAType::ChannelMode::Baseline;
    config6.startMass = 0;
    config6.stopMass = 0;
    config6.ppamu = 1;
    config6.channelType = RGAType::ChannelType::F32Value;
    configs.append(config6);
    ChannelConfig config7;
    config7.id = 7;
    config7.enabled = true;
    config7.channelMode = RGAType::ChannelMode::TPQuad;
    config7.startMass = 0;
    config7.stopMass = 0;
    config7.ppamu = 10;
    config7.channelType = RGAType::ChannelType::F32Value;
    configs.append(config7);
    ChannelConfig config8;
    config8.id = 8;
    config8.enabled = true;
    config8.channelMode = RGAType::ChannelMode::TotalPressureDetectorCurrent;
    config8.startMass = 0;
    config8.stopMass = 0;
    config8.ppamu = 10;
    config8.channelType = RGAType::ChannelType::F32Value;
    configs.append(config8);
    ChannelConfig config9;
    config9.id = 9;
    config9.enabled = true;
    config9.channelMode = RGAType::ChannelMode::EmissionCurrent;
    config9.startMass = 0;
    config9.stopMass = 0;
    config9.ppamu = 10;
    config9.channelType = RGAType::ChannelType::U32Value;
    configs.append(config9);
    ChannelConfig config10;
    config10.id = 10;
    config10.enabled = true;
    config10.channelMode = RGAType::ChannelMode::AnodePotential;
    config10.startMass = 0;
    config10.stopMass = 0;
    config10.ppamu = 10;
    config10.channelType = RGAType::ChannelType::U32Value;
    configs.append(config10);
    ChannelConfig config11;
    config11.id = 11;
    config11.enabled = true;
    config11.channelMode = RGAType::ChannelMode::ElectronEnergy;
    config11.startMass = 0;
    config11.stopMass = 0;
    config11.ppamu = 10;
    config11.channelType = RGAType::ChannelType::U32Value;
    configs.append(config11);
    ChannelConfig config12;
    config12.id = 12;
    config12.enabled = true;
    config12.channelMode = RGAType::ChannelMode::FocusPotential;
    config12.startMass = 0;
    config12.stopMass = 0;
    config12.ppamu = 10;
    config12.channelType = RGAType::ChannelType::U32Value;
    configs.append(config12);
    ChannelConfig config13;
    config13.id = 13;
    config13.enabled = true;
    config13.channelMode = RGAType::ChannelMode::FilamentCurrent;
    config13.startMass = 0;
    config13.stopMass = 0;
    config13.ppamu = 10;
    config13.channelType = RGAType::ChannelType::U32Value;
    configs.append(config13);

try {
        // 先更新通道
        ScanSetupChannel::instance()->updateChannels(SCAN_USER, configs);
        qDebug() << "通道更新完成";
        
        // 尝试打印通道信息
        qDebug() << "准备打印通道信息...";
        // ScanSetupChannel::instance()->printAllChannels(SCAN_USER);
        qDebug() << "通道信息打印完成";
    } catch (const std::exception& e) {
        qDebug() << "处理通道配置时发生异常:" << e.what();
    } catch (...) {
        qDebug() << "处理通道配置时发生未知异常";
    }

}

/*
---------------------------------
    单位类型改变槽函数，用于接收后端返回的单位类型乘数，然后更新单位类型乘数
---------------------------------
*/
void RGA::unitTypeChanged(QString unitType)
{
    QString retStr;
    QString err;
    auto requests = m_urlGenerator->findRequestsByUrlPart("sensorIonSource/get");
    if(requests.isEmpty())
    {
        qDebug() << "没有找到sensorIonSource/get的请求";
    }
    else
    {   
        qDebug() << "找到sensorIonSource/get的请求";
        if(HttpRequestSync(requests[0].url, retStr, err, 2000, ResponseFormat::JSON))
        {
            parseunitMultiplier(retStr);
            if(unitType == "Torr")
            {
                unitMultiplier = 1/(Sensitivity*FragFactor*IonizationProbability);
            }
            else if(unitType == "mB")
            {
                unitMultiplier = 1.333222/(Sensitivity*FragFactor*IonizationProbability);
            }
            else if(unitType == "Pa")
            {
                unitMultiplier = 133.3222/(Sensitivity*FragFactor*IonizationProbability);
            }
            else if(unitType == "PPM")
            {
                unitMultiplier = (1/(Sensitivity*FragFactor*IonizationProbability))*1000000;
            }
            qDebug() << "单位类型乘数:" << unitMultiplier;
            emit unitMultiplierChanged(unitMultiplier);

        }
    }

}

//解析单位类型转换所需要的数据
void RGA::parseunitMultiplier(const QString& response)
{
    bool ok;
    QJsonObject jsonObj = m_deviceMonitor->parseJsonString(response, &ok);
    if (!ok) {
        qDebug() << "Failed to parse JSON response";
        return;
    }
    else {
        Sensitivity = m_deviceMonitor->getJsonValue(jsonObj, "data.ppSensitivityFactor").toDouble();
        FragFactor = m_deviceMonitor->getJsonValue(jsonObj, "data.ppLinConst1").toDouble();
        IonizationProbability = m_deviceMonitor->getJsonValue(jsonObj, "data.ppLinConst2").toDouble();
        if(std::isnan(Sensitivity) || Sensitivity == 0)
        {
            Sensitivity = 1;
        }
        if(std::isnan(FragFactor) || FragFactor == 0)
        {
            FragFactor = 0.94;
        }
        if(std::isnan(IonizationProbability) || IonizationProbability == 0)
        {
            IonizationProbability = 1;
        }        
        qDebug() << "Sensitivity:" << Sensitivity;
        qDebug() << "FragFactor:" << FragFactor;
        qDebug() << "IonizationProbability:" << IonizationProbability;

    }

    //计算单位转换倍数

}

//监控测量状态
void RGA::onmeasurementStatusChanged(const QString& deviceId, const MeasurementStatus& status)
{
    qDebug() << "更新测量状态中的总压即可";
    m_currenttotalpressure = status.totalPressure;
    emit totalPressureChanged(m_currenttotalpressure);
}
