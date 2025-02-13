#include "DataFetcher.h"
#include "URLManger.h"


DataFetcher::DataFetcher(QObject *parent) : MyThread(parent)
    , nextPointIndex(0)
    , isFirst(true)

{
    setm_urlGenerator(nullptr);

    buffer = QList<QVariant>();

    setissequence(false);

    RequestFilter filter;
    filter.steps = { RequestStep::Invalid };
    setm_filter(filter);

    setisFirstConnect(false);

}

DataFetcher::~DataFetcher()
{
#ifdef TAO_DEBUG
    qDebug() << "DataFetcher::~DataFetcher()";
#endif
    stopThread();


}

void DataFetcher::processTask()
{
    QList<UrlRequest> requests = getm_urlGenerator()->getRequestsByFilter(getm_filter());
#ifdef TAO_DEBUG
    // qDebug() << "requests size" << requests.size();
#endif
    QString retStr;
    QString err;

    for(const UrlRequest& request : requests)
    {
#ifdef TAO_DEBUG
        // qDebug() << "request url:" << request.url;
#endif
        QString url = updateUrlParam(request.url, "@start", QString::number(getnextPointIndex()));
#ifdef TAO_DEBUG
        // qDebug() << "getnextPointIndex():" << getnextPointIndex();
        // qDebug() << "请求的url:" << url;
#endif
        if(HttpRequestSync(url, retStr, err, 3000, ResponseFormat::Binary))
        {
            if(request.needParse)
            {      
                // retStr 现在已经是十六进制字符串，直接转换
                QByteArray binaryData = QByteArray::fromHex(retStr.toUtf8());
#ifdef TAO_DEBUG
                // qDebug() << "Converted binary data size:" << binaryData.size() << "bytes";    
#endif
                if(!binaryData.isEmpty())
                {
                    // // 打印详细的二进制数据信息
                    // qDebug() << "Binary data in different formats:";
                    // qDebug() << "Raw hex format:" << binaryData.toHex();
#ifdef TAO_DEBUG
                    // if(getisFirst())
                    // {
                    //     QString byteString;
                    //     QString asciiString;

                    //     for(int i = 0; i < binaryData.size(); ++i) {
                    //         unsigned char byte = static_cast<unsigned char>(binaryData.at(i));
                    //         byteString += QString("%1 ").arg(byte, 2, 16, QChar('0'));
                    //         asciiString += (byte >= 32 && byte <= 126) ? QChar(byte) : '.';
                            
                    //         if ((i + 1) % 16 == 0 || i == binaryData.size() - 1) {
                    //             qDebug() << QString("%1: %2 | %3")
                    //                 .arg(i - (i % 16), 4, 16, QChar('0'))
                    //                 .arg(byteString.leftJustified(48, ' '))
                    //                 .arg(asciiString);
                    //             byteString.clear();
                    //             asciiString.clear();
                    //         }
                    //     }
                    // }
#endif
                    QList<ScanData> scans = parse(binaryData);

                    for(const ScanData& scan : scans)
                    {
                        QVariantMap scanMap = convertScanDataToMap(scan);
                        addToBuffer(scanMap);
                    }   
                }
            }
        }
    }

    QThread::msleep(800);
}   
QString DataFetcher::updateUrlParam(const QString& url, const QString& key, const QString& newValue)
{
    QUrl qUrl(url);
    QUrlQuery query(qUrl);
    
    // 如果参数存在，则更新值
    if (query.hasQueryItem(key)) {
        query.removeQueryItem(key);
        query.addQueryItem(key, newValue);
    }
    
    // 将更新后的查询参数设置回 URL
    qUrl.setQuery(query);
    
    return qUrl.toString();
}

void DataFetcher::setissequence(bool issequence)
{
    QMutexLocker locker(&m_mutex);
    this->issequence = issequence;
}

bool DataFetcher::getissequence()
{
    QMutexLocker locker(&m_mutex);
    return issequence;
}


bool DataFetcher::HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, int timeout, ResponseFormat format)
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
            request.setRawHeader("Accept", "application/octet-stream");
            break;
    }

    QNetworkAccessManager httpClient;
    QNetworkReply* reply = httpClient.get(request);

    bool okFinish = false;
    bool okTimeout = false;
    
    QEventLoop loop;
    QTimer timer;

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        timer.stop();
        if(reply->error() == QNetworkReply::NoError) {
            if (format == ResponseFormat::Binary) {
                // 对于二进制数据，先读取为 QByteArray
                QByteArray binaryData = reply->readAll();
                // 然后转换为十六进制字符串
                retStr = binaryData.toHex();
            } else {
                // 对于其他格式，直接读取为字符串
                retStr = reply->readAll();
            }
            okFinish = true;
        } else {
            err = reply->errorString();
        }
        loop.quit();
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

void DataFetcher::startscan()
{
    if (this->getState() == MyThread::ThreadState::Stopped) 
    {
        RequestFilter filter;
        filter.steps = { RequestStep::Step5 };
        setm_filter(filter);

        // 清空缓冲区
        clearBuffer();
        setnextPointIndex(0);
        setisFirst(true);

        this->startThread();

    }
    else if (this->getState() == MyThread::ThreadState::Paused) 
    {
        this->resumeThread();
    }
}

void DataFetcher::stopscan(bool isscan)
{
    if(this->getState() == MyThread::ThreadState::Running)
    {
        this->stopThread();
        
        RequestFilter filter;
        filter.steps = { RequestStep::Invalid };
        setm_filter(filter);

        // 清空缓冲区
        clearBuffer();
        setnextPointIndex(0);
        setisFirst(true);
        if(isscan)
        {
            emit stopscanSignal(isscan);
        }
        else
        {
            emit stopscanSignal(isscan);
        }
    }
    //  emit stopscanSignal();
}

void DataFetcher::setm_filter(const RequestFilter& filter)

{
    QMutexLocker locker(&m_mutex);
    this->m_filter = filter;
}

RequestFilter DataFetcher::getm_filter()
{
    QMutexLocker locker(&m_mutex);
    return m_filter;
}

void DataFetcher::setisFirstConnect(bool isFirstConnect)
{
    QMutexLocker locker(&m_mutex);
    this->isFirstConnect = isFirstConnect;

}
bool DataFetcher::getisFirstConnect()
{
    QMutexLocker locker(&m_mutex);
    return isFirstConnect;

}

void DataFetcher::setm_urlGenerator(UrlGenerator* m_urlGenerator)
{
    QMutexLocker locker(&m_mutex);
    this->m_urlGenerator = m_urlGenerator;
}

UrlGenerator* DataFetcher::getm_urlGenerator()
{
    QMutexLocker locker(&m_mutex);
    return m_urlGenerator;
}

void DataFetcher::setisFirst(bool isFirst)
{
    QMutexLocker locker(&m_mutex);
    this->isFirst = isFirst;
}

bool DataFetcher::getisFirst()
{
    QMutexLocker locker(&m_mutex);
    return isFirst;
}

void DataFetcher::setnextPointIndex(int nextPointIndex)
{
    QMutexLocker locker(&m_mutex);
    this->nextPointIndex = nextPointIndex;
}

int DataFetcher::getnextPointIndex()
{
    QMutexLocker locker(&m_mutex);
    return nextPointIndex;
}

// 解析字节数组的方法
QList<ScanData> DataFetcher::parse(const QByteArray& byteArray) 
{
    // 如果字节数组为空，则直接返回一个空的QVariant对象
    if (byteArray.isEmpty()) 
    {
        qDebug() << "parse - 字节数组为空。";
        return QList<ScanData>();
    }

    bool littleEndian = false; // 默认为大端序
    ScanData partialScan = {0};
    QList<ScanData> parsedScans = QList<ScanData>();

    // 使用BinaryConverter::readUInt32方法读取前4个字节作为消息头
    quint32 header = BinaryConverter::readUInt32(byteArray, littleEndian, 0);  // 从偏移量0开始读取4个字节
    if (header != HEADER_MARKER) {
        qDebug() << "parse - 消息头与HEADER_MARKER不匹配，尝试大端格式读取。";  // 如果消息头与预期的常量不匹配，输出提示信息
    } else {
        #ifdef TAO_DEBUG
            // qDebug() << "parse - 消息头匹配HEADER_MARKER。 通过";  // 如果消息头匹配预期的常量，输出成功信息
        #endif
    }

    // 读取 headerSize，偏移量为4，读取1个字节（表示头部的大小）
    quint8 headerSize = BinaryConverter::readUInt8(byteArray, littleEndian, 4); // 读取偏移量4处的一个字节

    // 读取 dataHeaderSize，偏移量为5，读取1个字节（表示数据头的大小）
    quint8 dataHeaderSize = BinaryConverter::readUInt8(byteArray, littleEndian, 5); // 读取偏移量5处的一个字节

    // 读取 dataSize，偏移量为6，读取2个字节（表示数据的大小）
    quint16 dataSize = BinaryConverter::readUInt16(byteArray, littleEndian, 6); // 读取偏移量6处的两个字节

    // 读取 dataType，偏移量为8，读取2个字节（表示数据的类型）
    quint16 dataType = BinaryConverter::readUInt16(byteArray, littleEndian, 8); // 读取偏移量8处的两个字节

    // 检查 dataType 是否匹配预期的BINARY_DATA_TYPE常量
    if (dataType != BINARY_DATA_TYPE) {
        qDebug() << "parse - 数据类型不匹配BINARY_DATA_TYPE。 不通过";  // 如果dataType与预期不匹配，输出错误信息
    } else {
        #ifdef TAO_DEBUG
            // qDebug() << "parse - 数据类型匹配BINARY_DATA_TYPE。 通过";  // 如果dataType匹配预期值，输出成功信息
        #endif
    }

    // 读取 reserved1，偏移量为10，读取2个字节（保留字段，通常用来对齐或未来扩展）
    quint16 reserved1 = BinaryConverter::readUInt16(byteArray, littleEndian, 10); 

    // 读取 reserved2，偏移量为12，读取4个字节（另一个保留字段）
    quint32 reserved2 = BinaryConverter::readUInt32(byteArray, littleEndian, 12); 

    // 读取 pointsSinceStartScan，偏移量为16，读取4个字节（表示从扫描开始到目前为止的点数）
    quint32 pointsSinceStartScan = BinaryConverter::readUInt32(byteArray, littleEndian, 16); 

    // 读取 scanSize，偏移量为20，读取4个字节（表示扫描的大小）
    quint32 scanSize = BinaryConverter::readUInt32(byteArray, littleEndian, 20); 

    // 读取 dataPointCount，偏移量为24，读取4个字节（表示数据点的数量）
    quint32 dataPointCount = BinaryConverter::readUInt32(byteArray, littleEndian, 24); 

    quint32 dataOffset = 28;  // 数据偏移量，从28开始


    // 计算 scanNum (表示当前扫描的编号,因为扫描不是从0开始的)
    // 计算当前扫描编号，使用 qFloor 来确保向下取整
    quint32 scanNum = qFloor(static_cast<qreal>(pointsSinceStartScan) / scanSize) + 1; 
    // 计算当前扫描的编号

    #ifdef TAO_DEBUG
        // qDebug() << "header (消息头):" << header;  // 输出读取到的消息头

        // qDebug() << "headerSize (头部大小):" << headerSize;  // 输出headerSize值
        // qDebug() << "dataHeaderSize (数据头大小):" << dataHeaderSize;  // 输出dataHeaderSize值
        // qDebug() << "dataSize (数据大小):" << dataSize;  // 输出dataSize值
        // qDebug() << "dataType (数据类型):" << dataType;  // 输出dataType值

        // qDebug() << "reserved1 (保留字段1):" << reserved1;  // 输出reserved1的值
        // qDebug() << "reserved2 (保留字段2):" << reserved2;  // 输出reserved2的值
        // qDebug() << "pointsSinceStartScan (扫描开始到目前为止的点数):" << pointsSinceStartScan;  // 输出pointsSinceStartScan的值
        // qDebug() << "scanSize (扫描大小):" << scanSize;  // 输出scanSize的值
        // qDebug() << "dataPointCount (数据点数量):" << dataPointCount;  // 输出dataPointCount的值
        // qDebug() << "scanNum (当前扫描编号):" << scanNum;  // 输出计算出来的scanNum
    #endif


    // 初始化变量
    quint32 idx = 0;                // 索引变量，可能用于后续的数据处理
    bool ranOutOfPoints = false;    // 用于标记是否数据点已经用完
    quint32 pointCount = 0;         // 存储数据点的数量

    bool oneEnabled = false;        // 用于标记是否存在启用的通道

    // 遍历 ScanSetupChannels，检查是否有启用的通道
    for (int sscIdx = 1; sscIdx <= ScanSetupChannel::instance()->getAllChannels(getparseChannelType()).size(); ++sscIdx) {
        // 获取当前通道的 `enabled` 属性，并进行检查

        // `get("enabled")` 返回当前通道的 "enabled" 属性的值，`.toBool()` 将其转换为布尔值
        if (ScanSetupChannel::instance()->getChannel(getparseChannelType(),sscIdx).enabled == true) {
            oneEnabled = true;  // 如果找到了启用的通道，设置 oneEnabled 为 true
            break;              // 找到一个启用的通道后退出循环
        }
    }
    // 如果没有启用的通道，则返回空值
    if (!oneEnabled) {
        // 如果没有启用的通道，返回 QVariant()，相当于 JavaScript 中的 null
        return QList<ScanData>();  // QVariant 是 Qt 中的通用数据类型，表示没有有效值
    }

    // 在这里，我们需要遍历数据点并确保
    // 如果这是第一次读取且 pointsSinceStartScan 非零
    if (getisFirst() && pointsSinceStartScan) {
        // 我们需要与最新的扫描对齐，因此需要确定当前的位置
        // 然后改变偏移量，以便这些点不会被处理
        quint32 pointsToPurge = scanSize - (pointsSinceStartScan % scanSize);  // 计算需要跳过的点数
        dataOffset += pointsToPurge * DATA_POINT_SIZE;  // 更新数据偏移量，跳过这些点
        pointCount += pointsToPurge;  // 增加点数计数，跳过的点数也要算进去
        scanNum += 1;  // 当前扫描编号加1，表示已经完成了一轮扫描
    }

    // 设置 isFirst 为 false，表示第一次处理已经结束
    setisFirst(false);

    // 当没有耗尽数据点并且扫描设置通道不为空时，继续处理
    while (!ranOutOfPoints && ScanSetupChannel::instance()->getAllChannels(getparseChannelType()).size()) 
    {
        // 遍历所有扫描设置通道
        for (int channelIdx = 1; channelIdx <= ScanSetupChannel::instance()->getAllChannels(getparseChannelType()).size() && !ranOutOfPoints; ++channelIdx)
        {
            #ifdef TAO_DEBUG
                // qDebug() << "开始处理通道:" << channelIdx;
                // qDebug() << "idx:" << idx;
                // qDebug() << "buffer.size():" << buffer.size();
            #endif
            // 获取当前通道
            const ChannelConfig &channel = ScanSetupChannel::instance()->getChannel(getparseChannelType(),channelIdx);

            // 如果当前通道未启用，则跳过该通道
            if (!channel.enabled) {
                qDebug() << "通道未启用:" << channelIdx;
                continue;
            }

            // 获取当前通道的模式
            QString channelMode = ScanSetupChannel::instance()->channelModeToString(channel.channelMode);

            // qDebug() << "通道模式:" << channelMode;

            // 如果通道模式是 "Sweep"，处理该通道数据
            if(channelMode == "Sweep")
            {
                // 计算当前通道总点数
                int totalChannelPoints = static_cast<int>(
                        (channel.stopMass - channel.startMass) * channel.ppamu + 1);

                // 遍历每个数据点，读取数据并存入缓冲区
                for (int i = 0; i < totalChannelPoints; ++i) 
                {
                    // 只有当 idx 超出 buffer 大小时才需要读取新数据
                    
                    if (idx >= buffer.size()) 
                    {
                        // qDebug() << "buffer.size():" << buffer.size();
                        // qDebug() << "byteArray.size():" << byteArray.size();
                        // 检查是否已经达到数据字节数组的结尾
                        if (dataOffset + DATA_POINT_SIZE > byteArray.size()) 
                        { 
                            ranOutOfPoints = true; // 如果数据点不足，标记为超出点数
                        } 
                        else
                        {
                            // 否则，读取数据并增加数据偏移量
                            buffer.push_back(BinaryConverter::readFloat32(byteArray, littleEndian, dataOffset).second);
                            dataOffset += DATA_POINT_SIZE;  // 增加数据偏移量
                            pointCount++;  // 增加数据点计数
                        }
                    }
                    idx++; // 增加当前索引
                }
            }
            // 如果通道模式是 "Single", "TPQuad", "TotalPressure", "Baseline"，处理这些特殊通道数据
            else if((channelMode == "Single")||(channelMode == "TPQuad")||(channelMode == "TotalPressure")||(channelMode == "Baseline")) 
            {
                // 如果索引超出缓冲区大小
                if (idx >= buffer.size()) 
                {
                    // 检查是否已经达到数据字节数组的结尾
                    if (dataOffset + DATA_POINT_SIZE > byteArray.size()) 
                    {
                        ranOutOfPoints = true;  // 如果数据点不足，标记为超出点数
                    } 
                    else 
                    {
                        // 否则，读取数据并增加数据偏移量
                        buffer.push_back(BinaryConverter::readFloat32(byteArray, littleEndian, dataOffset).second); // 读取浮动32位数据（即 double 类型数据）
                        dataOffset += DATA_POINT_SIZE;  // 增加数据偏移量
                        pointCount++;  // 增加数据点计数
                    }
                    #ifdef TAO_DEBUG
                        // qDebug() << "读取到的数据:" << buffer.last().toDouble();
                    #endif
                }
                idx++;  // 增加当前索引
            }
            // 其他类型的通道处理
            else 
            {
                // qDebug() << "其他类型的通道处理";
                // qDebug() << "准备读取数据 idx:" << idx;
                // qDebug() << "准备读取数据 buffer.size():" << buffer.size();
                // 如果索引超出缓冲区大小
                if (idx >= buffer.size()) 
                {
                    // 检查是否已经达到数据字节数组的结尾
                    if (dataOffset + DATA_POINT_SIZE > byteArray.size()) 
                    {
                        // qDebug() << "数据点不足";
                        ranOutOfPoints = true;  // 如果数据点不足，标记为超出点数
                    }
                    else 
                    {
                        // 如果通道类型为 F32Value，读取浮动32位数据；否则读取无符号32位数据
                        if(ScanSetupChannel::instance()->channelTypeToString(channel.channelType) == "F32Value")
                        {
                            buffer.push_back(BinaryConverter::readFloat32(byteArray, littleEndian, dataOffset).second);
                            #ifdef TAO_DEBUG
                                //  qDebug() << "读取到的数据:" << buffer.last().toDouble();
                            #endif
                        }
                        else 
                        {
                            buffer.push_back(BinaryConverter::readInt32(byteArray, littleEndian, dataOffset));
                            #ifdef TAO_DEBUG
                                //  qDebug() << "读取到的数据:" << buffer.last().toInt();
                            #endif
                        }
                        dataOffset += DATA_POINT_SIZE;  // 增加数据偏移量
                        pointCount++;  // 增加数据点计数

                    }
                }
                idx++; // 增加当前索引
            }
        }
#ifdef TAO_DEBUG
        // qDebug() << "buffer的大小:" << buffer.size();
        // qDebug() << "开始打包一帧数据";
#endif
        // 根据当前的扫描数据创建扫描属性
        ScanData scanAttributes = toAttributes(scanNum, scanSize, buffer);


        // 如果数据点不足
        if (ranOutOfPoints) 
        {
#ifdef TAO_DEBUG
            // qDebug() << "数据点不足";
            // qDebug() << "pointCount:" << pointCount;
            // qDebug() << "dataPointCount:" << dataPointCount;
#endif
            // 检查实际点数和头部指定的点数是否匹配
            if (pointCount != dataPointCount) {
                qCritical() << "Header point count does not match actual" << pointCount << dataPointCount;
            }

            // 处理不完整扫描的数据，保存扫描属性
            partialScan = scanAttributes;
            // 根据实际情况处理 partialScan（例如标记为不完整扫描）
        } 
        else 
        {
#ifdef TAO_DEBUG
            // 检查缓冲区的长度是否与扫描大小匹配
            // qDebug() << "buffer.size():" << buffer.size();
            // qDebug() << "scanSize:" << scanSize;
#endif
            if (buffer.size() != scanSize) 
            {
                qCritical() << "Buffer does not match scan size" 
                            << buffer.length() << scanSize 
                            << pointCount << dataPointCount;
            }
            // 将完整的扫描数据添加到解析结果中
            parsedScans.append(scanAttributes);

            // 增加扫描编号，准备处理下一个扫描
            scanNum++;  
            // 清空缓冲区以便下一次使用
            clearBuffer();
        }
    }

    setnextPointIndex(pointsSinceStartScan + pointCount);
#ifdef TAO_DEBUG
    // qDebug() << "parse - 解析方法结束。";
#endif
    // 返回一个空的QVariant对象（实际上这里应该包含解析后的数据）
    return parsedScans;

}

// 将扫描数据转换为属性格式的方法
ScanData DataFetcher::toAttributes(int scanNum, int scanSize, const QList<QVariant> buffer)
{
    quint32 idx = 0;
    Measurement measurement;  // 存储每个通道的测量数据


    // 创建 ScanData 对象
    ScanData scanData;
    scanData.id = scanNum;
    scanData.scanNum = scanNum;
    scanData.scanSize = scanSize;


    // 遍历每个扫描设置通道
    for (int channelIdx = 1; channelIdx <= ScanSetupChannel::instance()->getAllChannels(getparseChannelType()).size(); channelIdx++)
    {
        // 获取当前通道
        const ChannelConfig &channel = ScanSetupChannel::instance()->getChannel(getparseChannelType(),channelIdx);

        // 只有在通道启用且有数据时才处理
        if (!channel.enabled || idx >= buffer.size()) {
            #ifdef TAO_DEBUG
                // qDebug() << "toAttributes - 通道未启用或索引超出范围，跳过该通道。";
            #endif
            continue;
        }

        // 根据通道模式计算需要提取的数据点数量
        QString channelMode = ScanSetupChannel::instance()->channelModeToString(channel.channelMode);
        if (channelMode == "Sweep") 
        {    
            // 计算当前通道需要提取的数据点总数
            int channelTotalPoints = static_cast<int>(((channel.stopMass - channel.startMass) * channel.ppamu) + 1);
            measurement.channelId = channel.id;
            measurement.stopMass = channel.stopMass;
            measurement.startMass = channel.startMass;
            measurement.ppamu = channel.ppamu;
            
            // 将 QVariant 列表转换为 double 列表
            QList<QVariant> channelData = buffer.mid(idx, channelTotalPoints);
            for (const QVariant& value : channelData) {
                measurement.data.append(value.toDouble());
            }
        
            idx += channelTotalPoints;
        }
        // 处理SINGLE模式的通道
        else if (channelMode == "Single")
        {
            measurement.channelId = channel.id;
            measurement.stopMass = channel.stopMass;
            measurement.startMass = channel.startMass;
            measurement.ppamu = channel.ppamu;
            measurement.data.append(buffer[idx].toDouble());
            idx += 1;
        }
        // 处理其他模式的通道
        else
        {
            if(channelMode == "FixedNumber")
            {
                scanData.FixedNumber = static_cast<qint32>(buffer[idx].toInt());
                idx += 1;
            }
            else if(channelMode == "Timestamp")
            {
                scanData.Timestamp = static_cast<qint64>(buffer[idx].toInt());
                idx += 1;
            }
            else if(channelMode == "SystemStatus")

            {
                scanData.SystemStatus = static_cast<qint32>(buffer[idx].toInt());
                idx += 1;
            }

            else if(channelMode == "TotalPressure")

            {
                scanData.TotalPressure = buffer[idx].toDouble();
                idx += 1;
            }
            else if(channelMode == "Baseline")

            {
                scanData.Baseline = buffer[idx].toDouble();
                idx += 1;
            }
            else if(channelMode == "TPQuad")

            {
                scanData.TPQuad = buffer[idx].toDouble();
                idx += 1;
            }
            else if(channelMode == "TotalPressureDetectorCurrent")

            {
                scanData.TotalPressureDetectorCurrent = buffer[idx].toDouble();
                idx += 1;
            }
            else if(channelMode == "EmissionCurrent")

            {
                scanData.EmissionCurrent = buffer[idx].toInt();
                idx += 1;
            }
            else if(channelMode == "AnodePotential")

            {
                scanData.AnodePotential = buffer[idx].toInt();
                idx += 1;
            }
            else if(channelMode == "ElectronEnergy")

            {
                scanData.ElectronEnergy = buffer[idx].toInt();
                idx += 1;
            }
            else if(channelMode == "FocusPotential")

            {
                scanData.FocusPotential = buffer[idx].toInt();
                idx += 1;
            }
            else if(channelMode == "FilamentCurrent")

            {
                scanData.FilamentCurrent = buffer[idx].toInt(); 
                idx += 1;
            }
            else

            {
                qDebug() << "toAttributes - 未知的通道模式，跳过该通道。";
                continue;
            }
        }
    }
    // 将测量数据添加到 ScanData 对象中
    scanData.measurements.append(measurement);

    return scanData;
}

// 封装为一个函数，传入扫描数据并返回 QVariantMap
QVariantMap DataFetcher::convertScanDataToMap(const ScanData &scanData) 
{
    QVariantMap scanDataMap;
    scanDataMap["scanNum"] = scanData.scanNum;

    scanDataMap["scanSize"] = scanData.scanSize;
    scanDataMap["id"] = scanData.id;
    scanDataMap["FixedNumber"] = scanData.FixedNumber;
    scanDataMap["Timestamp"] = scanData.Timestamp;
    scanDataMap["SystemStatus"] = scanData.SystemStatus;
    scanDataMap["TotalPressure"] = scanData.TotalPressure;
    scanDataMap["Baseline"] = scanData.Baseline;
    scanDataMap["TPQuad"] = scanData.TPQuad;

    scanDataMap["TotalPressureDetectorCurrent"] = scanData.TotalPressureDetectorCurrent;
    scanDataMap["EmissionCurrent"] = scanData.EmissionCurrent;
    scanDataMap["AnodePotential"] = scanData.AnodePotential;
    scanDataMap["ElectronEnergy"] = scanData.ElectronEnergy;
    scanDataMap["FocusPotential"] = scanData.FocusPotential;
    scanDataMap["FilamentCurrent"] = scanData.FilamentCurrent;


    // 将测量数据转换为 QVariantList
    QVariantList measurementsList;
    for (const Measurement &measurement : scanData.measurements) {
        QVariantMap measurementMap;
        measurementMap["channelId"] = measurement.channelId;
        measurementMap["stopMass"] = measurement.stopMass;
        measurementMap["startMass"] = measurement.startMass;
        measurementMap["ppamu"] = measurement.ppamu;
        measurementMap["data"] = QVariant::fromValue(measurement.data);  // 存储浮点数据列表
        measurementsList.append(measurementMap);
    }

    scanDataMap["measurements"] = measurementsList;

    return scanDataMap;
}

void DataFetcher::printScanData(const QVariantMap &scanDataMap) 
{
    // 打印基本数据类型
    qDebug() << "Scan Number: " << scanDataMap["scanNum"].toInt();

    qDebug() << "Scan Size: " << scanDataMap["scanSize"].toInt();
    qDebug() << "ID: " << scanDataMap["id"].toInt();
    qDebug() << "Fixed Number: " << scanDataMap["FixedNumber"].toUInt();
    qDebug() << "Timestamp: " << scanDataMap["Timestamp"].toUInt();
    qDebug() << "System Status: " << scanDataMap["SystemStatus"].toInt();
    qDebug() << "Total Pressure: " << scanDataMap["TotalPressure"].toDouble();
    qDebug() << "Baseline: " << scanDataMap["Baseline"].toDouble();
    qDebug() << "TPQuad: " << scanDataMap["TPQuad"].toDouble();
    qDebug() << "Total Pressure Detector Current: " << scanDataMap["TotalPressureDetectorCurrent"].toDouble();
    qDebug() << "Emission Current: " << scanDataMap["EmissionCurrent"].toInt();
    qDebug() << "Anode Potential: " << scanDataMap["AnodePotential"].toInt();
    qDebug() << "Electron Energy: " << scanDataMap["ElectronEnergy"].toInt();
    qDebug() << "Focus Potential: " << scanDataMap["FocusPotential"].toInt();
    qDebug() << "Filament Current: " << scanDataMap["FilamentCurrent"].toInt();

    // // 打印 measurements
    // QVariantList measurementsList = scanDataMap["measurements"].toList();
    // for (const QVariant &measurementVariant : measurementsList) {
    //     QVariantMap measurementMap = measurementVariant.toMap();

    //     int channelId = measurementMap["channelId"].toInt();
    //     double stopMass = measurementMap["stopMass"].toDouble();
    //     double startMass = measurementMap["startMass"].toDouble();
    //     double ppamu = measurementMap["ppamu"].toDouble();
    //     QList<double> data = measurementMap["data"].value<QList<double>>();

    //     // 打印每个测量数据项
    //     qDebug() << "Measurement Info:";
    //     qDebug() << "  ChannelId: " << channelId;
    //     qDebug() << "  StopMass: " << stopMass;
    //     qDebug() << "  StartMass: " << startMass;
    //     qDebug() << "  Ppamu: " << ppamu;
    //     qDebug() << "  Data: ";
    //     for (double value : data) {
    //         qDebug() << "    " << value;
    //     }
    // }
}

void DataFetcher::addToBuffer(const QVariantMap& data)
{
    QMutexLocker locker(&m_mutex);  // 使用互斥锁保护
    
    // 添加新数据到缓冲区
    if (m_dataBuffer.size() >= MAX_BUFFER_SIZE) {
        m_dataBuffer.dequeue();  // 如果缓冲区满了，移除最旧的数据
    }
    m_dataBuffer.enqueue(data);
#ifdef TAO_DEBUG
    printScanData(data);
#endif
    
    emit newDataAvailable(data);
}

QList<QVariantMap> DataFetcher::getBufferData(int count)
{
    QMutexLocker locker(&m_mutex);  // 使用已有的 m_mutex
    QList<QVariantMap> result;
    
    int actualCount = qMin(count, m_dataBuffer.size());
    
    for (int i = 0; i < actualCount; ++i) {
        result.append(m_dataBuffer.dequeue());
    }
    
    return result;
}

void DataFetcher::clearDataBuffer()
{
    QMutexLocker locker(&m_mutex);  // 使用已有的 m_mutex
    m_dataBuffer.clear();
}

void DataFetcher::clearBuffer()
{
    QMutexLocker locker(&m_mutex);  // 使用已有的 m_mutex
    buffer.clear();
}


void DataFetcher::setparseChannelType(int parseChannelType)

{
    QMutexLocker locker(&m_mutex);
    m_parseChannelType = parseChannelType;
}

int DataFetcher::getparseChannelType() 
{
    QMutexLocker locker(&m_mutex);
    return m_parseChannelType;
}

