#include "DataFetcher.h"
#include <qdebug.h>
#include <qlogging.h>
#include <qmutex.h>
#include <qthread.h>
#include <qvariant.h>



DataFetcher::DataFetcher(UrlGenerator* urlGenerator)
    : m_urlGenerator(urlGenerator), exitCondition(false),
      isConnect(false)
      ,isScanDevice(false)
      , isScan(false)
      ,issequence(false)
      , currentStep(RequestStep::Invalid)
      ,m_scanMessageParser(new ScanMessageParser())
       {

       }

DataFetcher::~DataFetcher() 
{
    stopFetching();  // 确保在销毁对象时线程已停止    
    quit();  // 退出线程
    wait();  // 等待线程结束
    delete m_scanMessageParser;

}

void DataFetcher::startFetching() {

    if(this->isRunning())
    {
        return;
    }
    exitCondition = false;
    start();  // 启动线程，自动调用 run()
}

void DataFetcher::stopFetching() 
{   
    exitCondition = true;
}

void DataFetcher::pauseFetching() 
{
    setpauseStep();
}


void DataFetcher::processStep1() 
{
    // QList<UrlRequest> requests = m_urlGenerator->getRequestsByFilter(m_filter);
    // if(issequence)
    // {
    //     issequence = false;
    //     requests = sortRequestsByPriority(requests);
    //     qDebug() << "sort requests by priority";
    //     for(const UrlRequest& request : requests)
    //     {
    //         qDebug() << "request url:" << request.url;
    //     }
    // }
    // bool isHave = m_urlGenerator->findUrlInRequests(requests,"@start");
    // if(isHave)
    // {
    //     m_format = ResponseFormat::Binary;
    // }
    // else
    // {
    //     m_format = ResponseFormat::JSON;
    // }
    
    QString retStr;
    QString err;

    // for(const UrlRequest& request : requests)
    // {
    //     qDebug() << "request url:" << request.url;

    //     HttpRequestSync(request.url, retStr, err, 10000, m_format);
    // }

    // qDebug() << "task is running..." << QThread::currentThreadId();

    HttpRequestSync("http://jsonplaceholder.typicode.com/posts/1", retStr, err, 10000, ResponseFormat::JSON);
    HttpRequestSync("http://jsonplaceholder.typicode.com/posts/2", retStr, err, 10000, ResponseFormat::JSON);
    HttpRequestSync("http://jsonplaceholder.typicode.com/posts/3", retStr, err, 10000, ResponseFormat::JSON);
    QThread::msleep(1000);

}

void DataFetcher::processStep2() 
{
    QList<UrlRequest> requests = m_urlGenerator->getRequestsByFilter(m_filter);
    qDebug() << "requests size" << requests.size();
    if(issequence)
    {
        issequence = false;
        requests = sortRequestsByPriority(requests);
        qDebug() << "sort requests by priority";
        for(const UrlRequest& request : requests)
        {
            qDebug() << "request url:" << request.url;
        }
    }
    bool isHave = m_urlGenerator->findUrlInRequests(requests,"@start");
    qDebug() << "isHave" << isHave;
    if(isHave)
    {
        m_format = ResponseFormat::Binary;
    }
    else
    {
        m_format = ResponseFormat::JSON;
    }
    
    QString retStr;
    QString err;

    // for(const UrlRequest& request : requests)
    // {
    //     qDebug() << "request url:" << request.url;

    //     HttpRequestSync(request.url, retStr, err, 10000, m_format);
    // }

    
    qDebug() << "processStep2 is running..." << QThread::currentThreadId();
    QThread::msleep(1000);
}

void DataFetcher::processStep3() 
{

}

void DataFetcher::processStep4() 
{

}

void DataFetcher::processStep5() 
{
    
}



void DataFetcher::run() {
#ifdef TAO_DEBUG
    qDebug() << "数据交互线程 is running " << QThread::currentThreadId();
#endif
    while (!exitCondition) 
    {
        switch (getStep()) {
        case RequestStep::Step1:
                processStep1();
            break;
        case RequestStep::Step2:
                processStep2();
            break;
        case RequestStep::Step3:
                processStep3();
            break;
        case RequestStep::Step4:
            processStep4();
            break;
        case RequestStep::Step5:
            processStep5();
            break;
        default:
            QThread::msleep(100);  // 等待状态变化
            break;
        }

        QCoreApplication::processEvents();  // 处理事件
    }
}


//设置当前交互步骤
void DataFetcher::setStep1()
{
    QMutexLocker locker(&mutex);
    currentStep = RequestStep::Step1;
}

//设置当前交互步骤,外部已经加锁，外部调用不能重复加锁，不然死锁
void DataFetcher::setStep2()
{
    currentStep = RequestStep::Step2;
}

//设置当前交互步骤
void DataFetcher::setStep3()
{
    QMutexLocker locker(&mutex);
    currentStep = RequestStep::Step3;
}

//设置当前交互步骤
void DataFetcher::setStep4()
{
    QMutexLocker locker(&mutex);
    currentStep = RequestStep::Step4;
}

//设置当前交互步骤
void DataFetcher::setStep5()
{
    QMutexLocker locker(&mutex);
    currentStep = RequestStep::Step5;
}

//设置当前交互步骤
void DataFetcher::setpauseStep()
{
    QMutexLocker locker(&mutex);
    currentStep = RequestStep::Invalid;
}


//   获取当前交互步骤
RequestStep DataFetcher::getStep()
{
    QMutexLocker locker(&mutex);
    return currentStep;
}

// 获取 scanSetupChannels 变量（线程安全）解析要用
QList<ScanSetupChannel> DataFetcher::getScanSetupChannels() const
{
    return m_scanMessageParser->getScanSetupChannels();
}

// 设置 scanSetupChannels 变量（线程安全）解析要用
void DataFetcher::setScanSetupChannels(const QList<ScanSetupChannel>& channels)
{
    m_scanMessageParser->setScanSetupChannels(channels);
}

bool DataFetcher::getJsonField(const QString &jsonStr)
{
    qDebug() << "start parse....";
    QString field;
    QString fieldType;

    //解析scanning字段，判断设备是否正在扫描
    QVariant posts =  m_scanMessageParser->getJsonField(jsonStr,"scanning");
    if(posts.isValid())
    {   
        if(posts.canConvert<bool>())
        {

            bool isScandevice = posts.toBool();
            //保存第一次状态
            setScanDevice(isScandevice);
    #ifdef TAO_DEBUG                        
            qDebug() << "scandevice " << isScandevice;
    #endif
        }
    }
    //解析controled字段，判断设备是否正在控制
    posts =  m_scanMessageParser->getJsonField(jsonStr,"controlled");
    if(posts.isValid())
    {
        if(posts.canConvert<bool>())
        {
            bool isScandevice = posts.toBool();
    #ifdef TAO_DEBUG                        
            qDebug() << "controlled " << isScandevice;
    #endif
        }
    }
    //解析setEmission字段，判断设备是否正在控制
    posts =  m_scanMessageParser->getJsonField(jsonStr,"setEmission");
    if(posts.isValid())
    {
        if(posts.canConvert<QString>())
        {
            QString isScandevice = posts.toString();
    #ifdef TAO_DEBUG                        
            qDebug() << "setEmission " << isScandevice;
    #endif
        }
    }
    
    qDebug() << "parse complete....";
    return true;
} 

void DataFetcher::startScan()
{

    setStep2();
    RequestFilter filter;
    filter.steps = { RequestStep::Step2 };
    setm_filter(filter);
}

//主要设置url属性，发送停止扫描
void DataFetcher::stopScan()
{
    setStep1();
    emit stopscanSignal();
    RequestFilter filter;
    filter.steps = { RequestStep::Step1 };
    setm_filter(filter);
}

void DataFetcher::startconnect()
{
    startFetching();
    setStep1();
    RequestFilter filter;
    filter.steps = { RequestStep::Step1 };
    setm_filter(filter);
}

void DataFetcher::stopconnect()
{
    stopFetching();
    emit stopconnectSignal();
    RequestFilter filter;
    filter.steps = { RequestStep::Invalid };
    setm_filter(filter);
}


void DataFetcher::setConnectStatus(QString ip, int port,bool checked)
{
    QMutexLocker locker(&mutex);
    this->isConnect = checked;
}

void DataFetcher::setScanStatus(QString startMass,QString stopMass,QString ppamu,QString dwell,bool isScan)
{
    QMutexLocker locker(&mutex);
    this->isScan = isScan;
}

bool DataFetcher::getConnectStatus()
{
    QMutexLocker locker(&mutex);
    return isConnect;
}
bool DataFetcher::getScanStatus()
{
    QMutexLocker locker(&mutex);
    return isScan;
}

void DataFetcher::setScanDevice(bool isScanDevice)
{
    QMutexLocker locker(&mutex);
    this->isScanDevice = isScanDevice;
}


bool DataFetcher::getScanDevice()
{
    QMutexLocker locker(&mutex);
    return isScanDevice;
}
void DataFetcher::setissequence(bool issequence)
{
    QMutexLocker locker(&mutex);
    this->issequence = issequence;
}

bool DataFetcher::getissequence()
{
    QMutexLocker locker(&mutex);
    return issequence;
}
void DataFetcher::setm_filter(const RequestFilter& filter)
{
    QMutexLocker locker(&mutex);
    this->m_filter = filter;
}

RequestFilter DataFetcher::getm_filter()
{
    QMutexLocker locker(&mutex);
    return m_filter;
}

bool DataFetcher::HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, int timeout, ResponseFormat format)
{
     qDebug() << "HttpRequestSync is running..." << QThread::currentThreadId();
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
            {
                QJsonDocument doc = QJsonDocument::fromJson(retStr.toUtf8());
                if (doc.isObject()) {
                    QJsonObject obj = doc.object();
                    // 处理 JSON 数据
                    qDebug() << "Received JSON:" << obj;
                } else {
                    err = "Invalid JSON response";
                }
                break;
            }
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

bool DataFetcher::HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, ResponseFormat format)
{
    qDebug() << "HttpRequestSync is running..." << QThread::currentThreadId();
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

    QEventLoop loop; // 自定义事件循环

    // 响应处理
    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            retStr = reply->readAll(); // 获取响应内容
            okFinish = true; // 请求成功
        } else {
            err = reply->errorString();
        }
        loop.quit(); // 退出事件循环
    });

    loop.exec(); // 进入事件循环，等待请求完成

    reply->deleteLater(); // 确保资源释放

    // 解析返回数据（根据格式不同进行处理）
    if (okFinish) {
        switch (format) {
            case ResponseFormat::JSON: {
                QJsonDocument doc = QJsonDocument::fromJson(retStr.toUtf8());
                if (doc.isObject()) {
                    QJsonObject obj = doc.object();
                    // 处理 JSON 数据
                    qDebug() << "Received JSON:" << obj;
                } else {
                    err = "Invalid JSON response";
                }
                break;
            }
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

    return okFinish; // 返回请求是否成功
}

QList<UrlRequest> DataFetcher::sortRequestsByPriority(const QList<UrlRequest>& requests) 
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

