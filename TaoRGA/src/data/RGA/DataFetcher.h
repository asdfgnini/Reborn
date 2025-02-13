#pragma once


#include "../Thread/mythread.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include "UrlGenerator.h"
#include "DeviceMonitor.h"
#include <QtConcurrent/QtConcurrent>
#include "ScanSetupChannel.h"
#include <QList>



class BinaryConverter {
public:
    /**
     * 从字节数组中读取一个整数。
     * @param byteArray - 要转换的字节数组。
     * @param littleEndian - 如果为true，则是小端序，否则为大端序。
     * @param numOfBytes - 要解析的字节数。
     * @param offset - 字节数组中要解析的偏移量。
     * @return 整数。
     */
    // 小端序读取32位无符号整数
    static quint32 readUInt32(const QByteArray &byteArray, bool littleEndian, int offset = 0) {
        quint32 value = 0;
        if (littleEndian) {
            value = *reinterpret_cast<const quint32*>(byteArray.constData() + offset);
            value = qFromLittleEndian(value);  // 小端序转换
        } else {
            value = *reinterpret_cast<const quint32*>(byteArray.constData() + offset);
            value = qFromBigEndian(value);  // 大端序转换
        }
        return value;
    }
    /**
     * 从字节数组中读取一个有符号32位整数。
     * @param byteArray - 要转换的字节数组。
     * @param littleEndian - 如果为true，则是小端序，否则为大端序。
     * @param offset - 字节数组中要解析的偏移量。
     * @return 有符号整数。
     */
    static qint32 readInt32(const QByteArray &byteArray, bool littleEndian, int offset = 0) {
        qint32 value = 0;
        if (littleEndian) {
            value = *reinterpret_cast<const qint32*>(byteArray.constData() + offset);
            value = qFromLittleEndian(value);  // 小端序转换
        } else {
            value = *reinterpret_cast<const qint32*>(byteArray.constData() + offset);
            value = qFromBigEndian(value);  // 大端序转换
        }
        return value;
    }

    static quint16 readUInt16(const QByteArray &byteArray, bool littleEndian, int offset = 0) 
    {
        quint16 value = 0;
        if (littleEndian) {
            value = *reinterpret_cast<const quint16*>(byteArray.constData() + offset);
            value = qFromLittleEndian(value);
        } else {
            value = *reinterpret_cast<const quint16*>(byteArray.constData() + offset);
            value = qFromBigEndian(value);
        }
        return value;
    }

    static quint8 readUInt8(const QByteArray &byteArray, bool littleEndian, int offset = 0) {
        return static_cast<quint8>(byteArray[offset]);
    }

    /**
    * 从字节数组中读取一个32位浮点数。
    * @param byteArray - 要转换的字节数组。
    * @param littleEndian - 如果为true，则是小端序，否则为大端序。
    * @param offset - 字节数组中要解析的偏移量。
    * @return 浮点数。
    */
    // 从字节数据中手动提取浮动32位数值并转换为科学计数法表示
    static QPair<QString, float> readFloat32(const QByteArray &byteArray, bool littleEndian, int offset = 0) {
        // 获取字节数组中指定偏移量的4个字节
        QByteArray tempArray = byteArray.mid(offset, 4);

    #ifdef TAO_DEBUG   
        // 输出提取的字节数据（用于调试）
        // qDebug() << "Extracted bytes:" << tempArray.toHex();
    #endif
        if (tempArray.size() < 4) 
        {
            qWarning() << "Not enough bytes to extract a 32-bit float!";
            return qMakePair(QString("Error"), 0.0f);
        }

        // 按字节顺序将字节数组转换为浮动值
        uint32_t rawData = 0;
        if (littleEndian) {
            rawData = (static_cast<uint32_t>(static_cast<unsigned char>(tempArray[0])) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(tempArray[1])) << 8) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(tempArray[2])) << 16) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(tempArray[3])) << 24));
        } else {
            rawData = (static_cast<uint32_t>(static_cast<unsigned char>(tempArray[3])) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(tempArray[2])) << 8) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(tempArray[1])) << 16) |
                    (static_cast<uint32_t>(static_cast<unsigned char>(tempArray[0])) << 24));
        }
    #ifdef TAO_DEBUG
        // 输出解析后的原始数据
        // qDebug() << "Raw data (hex):" << QString::number(rawData, 16);
    #endif
        // 解析 IEEE 754 浮动点格式
        int sign = (rawData >> 31) ? -1 : 1;
        int exponent = ((rawData >> 23) & 0xFF) - 127;  // 8 bits for exponent, subtract bias 127
        uint32_t mantissa = (rawData & 0x7FFFFF);  // 23 bits for mantissa
        float result;

        // 如果指数不是0，尾数加上隐式的 1
        if (exponent != -127) {
            result = sign * (1 + static_cast<float>(mantissa) / (1 << 23)) * std::pow(2, exponent);
        } else {
            // 如果指数是0，表示它是一个次正规数，尾数前加上隐式 0
            result = sign * static_cast<float>(mantissa) / (1 << 23) * std::pow(2, -126); // 2^-126
        }
    #ifdef TAO_DEBUG
        // 输出读取的浮动值
        // qDebug() << "Read float value:" << result;
    #endif
        // 计算科学计数法字符串
        QString scientificNotation = QString::number(result, 'e', 15); // 'e'表示科学计数法，保留15位精度

        return qMakePair(scientificNotation, result);
    }

};


// 定义常量，用于解析二进制数据时的边界和标识
#define WORD_SIZE 4 // 每个字的大小，通常是 4 字节
#define BINARY_DATA_TYPE 0x44 // 二进制数据类型的标识
#define HEADER_MARKER 0x0A0B0C0D // 头部标记，用于识别消息的开始
#define HEADER_MARKER_SIZE 4 // 头部标记的大小，通常是 4 字节
#define HEADER_SIZE_SIZE 1 // 头部大小的大小，通常是 1 字节
#define HEADER_DATA_HEADER_SIZE 1 // 数据头部大小的大小，通常是 1 字节
#define HEADER_DATA_SIZE_SIZE 2 // 数据大小的大小，通常是 2 字节
#define HEADER_DATA_TYPE_SIZE 2 // 数据类型的大小，通常是 2 字节
#define DATA_HEADER_POINT_TOTAL_SIZE 4 // 数据头部中点总数的大小，通常是 4 字节
#define DATA_HEADER_SCAN_SIZE_SIZE 4 // 数据头部中扫描大小的大小，通常是 4 字节
#define DATA_HEADER_COUNT_SIZE 4 // 数据头部中数据点计数的大小，通常是 4 字节
#define DATA_POINT_SIZE 4 // 每个数据点的大小，通常是 4 字节


// 定义一个结构来表示测量数据的项
struct Measurement {
    int channelId;   // 通道ID
    double stopMass; // 终止质量
    double startMass; // 起始质量
    double ppamu;    // 每个质谱单位的质量
    QList<double> data;  // 数据点列表
};

// 定义一个结构来表示扫描数据
struct ScanData {
    uint64_t scanNum;    // 扫描编号
    uint64_t scanSize;   // 扫描大小
    uint64_t id;         // ID
    qint32 FixedNumber;  // 固定数字
    qint32 Timestamp;    // 时间戳
    qint32 SystemStatus;     // 系统状态
    double TotalPressure; // 总压力

    double Baseline;      // 基线
    double TPQuad;        // 四分之一压力

    double TotalPressureDetectorCurrent;  // 总压力探测器电流
    qint32 EmissionCurrent;  // 发射电流
    qint32 AnodePotential;   // 阳极电位
    qint32 ElectronEnergy;   // 电子能量
    qint32 FocusPotential;   // 聚焦电位
    qint32 FilamentCurrent;  // 灯丝电流
    QList<Measurement> measurements; // 测量数据

};

class DataFetcher : public MyThread
{
    Q_OBJECT
public:
    explicit DataFetcher(QObject *parent = nullptr);
    ~DataFetcher();

    void addToBuffer(const QVariantMap& data);
    QList<QVariantMap> getBufferData(int count = 10);
    void clearDataBuffer();
    void clearBuffer();
    
    // 解析二进制数据
    QList<ScanData> parse(const QByteArray& byteArray);

    // 将扫描数据转换为QVariantMap
    QVariantMap convertScanDataToMap(const ScanData &scanData);

    // 打印扫描数据
    void printScanData(const QVariantMap &scanDataMap);

    void setissequence(bool issequence);
    bool getissequence();



    void setm_filter(const RequestFilter& filter);
    RequestFilter getm_filter();

    void setisFirstConnect(bool isFirstConnect);
    bool getisFirstConnect();


    void setm_urlGenerator(UrlGenerator* m_urlGenerator);
    UrlGenerator* getm_urlGenerator();

    void setisFirst(bool isFirst);
    bool getisFirst();

    void setnextPointIndex(int nextPointIndex);
    int getnextPointIndex();

    void setparseChannelType(int parseChannelType);
    int getparseChannelType();

    QString updateUrlParam(const QString& url, const QString& key, const QString& newValue);

signals:
    void dataReceived(QJsonObject);

    void dataError(QString);

    void stopscanSignal(bool isscan);

    void newDataAvailable(QVariantMap data);

public slots:

    void startscan();
    void stopscan(bool isscan);

protected:
    void processTask() override;

private:
    // 将扫描数据转换为属性的函数
    ScanData toAttributes(int scanNum, int scanSize, const QList<QVariant> buffer);
    bool HttpRequestSync(const QString& urlStr, QString& retStr, QString& err, int timeout, ResponseFormat format);
    QList<UrlRequest> sortRequestsByPriority(const QList<UrlRequest>& requests);

private:
    QMutex m_mutex;
    static const int MAX_BUFFER_SIZE = 1000;
    QQueue<QVariantMap> m_dataBuffer;  // 只添加这一行，使用已有的 m_mutex
    // 扫描消息解析器
    UrlGenerator* m_urlGenerator;
    //是否进行排序
    bool issequence;
    // 请求过滤器
    RequestFilter m_filter;
    //这个isFirstConnect表示的是需要更改一次获取条件,这里只需要更改一次获取条件，所以用bool类型，如果需要更改多次获取条件，可以使用int来记录，比如说int cnt = 2表示要更改二次获取条件
    //然后在processTask中判断cnt是否大于0，如果大于0，则更改获取条件，cnt--，否则不更改获取条件
    bool isFirstConnect;

    // 表示第一次扫描
    bool isFirst = true;
    
    // 表示下一个数据点的索引
    uint64_t nextPointIndex = 0;
    
    // 缓冲区 用于将每个通道的数据点提取出来，保存到这里，同时如果返回的是部分扫描的数据，则需要将数据保存到这里，然后进行合并
    //因为需要等待下次扫描的数据，所以设为全局，同时因为是全局，所以结束扫描的时候，需要清空缓冲区
    QList<QVariant> buffer;

    int m_parseChannelType;
};

/*
    convertScanDataToMap测试代码：
*/
//  // 创建 ScanData 对象并填充数据
//     ScanData scanData;
//     scanData.scanNum = 756;
//     scanData.scanSize = 503;
//     scanData.id = 756;
//     scanData.fixedNumber = 1234567;
//     scanData.timestamp = 3956830;
//     scanData.systemStatus = -2096088174;
//     scanData.totalPressure = 0.000001975969780687592;
//     scanData.baseline = 1.5657554801860353e-14;
//     scanData.tpQuad = 3.5283129751206843e-9;
//     scanData.totalPressureDetectorCurrent = 2.8148219310963896e-9;
//     scanData.emissionCurrent = 2018;
//     scanData.anodePotential = 25095;
//     scanData.electronEnergy = 6988;
//     scanData.focusPotential = 3000;
//     scanData.filamentCurrent = 2931;

//     // 创建 Measurement 对象并填充数据
//     Measurement measurement;
//     measurement.channelId = 4;
//     measurement.stopMass = 50;
//     measurement.startMass = 1;
//     measurement.ppamu = 10;
    
//     // 假设 data 是一组浮点数据（可以根据实际情况填充）
//     measurement.data = {
//         5.144135985235554e-12, 4.6512706880497134e-12, 2.3574204654347453e-12,
//         2.178612542105829e-12, 4.4758758331597814e-12, 1.671777646872208e-11
//         // ... 其余数据
//     };
    
//     // 将测量数据添加到扫描数据中
//     scanData.measurements.append(measurement);

//     // 使用函数转换数据
//     QVariantMap scanDataMap = parser.convertScanDataToMap(scanData);
// // 打印基本数据类型
// qDebug() << "Scan Number: " << scanDataMap["scanNum"].toInt();
// qDebug() << "Scan Size: " << scanDataMap["scanSize"].toInt();
// qDebug() << "ID: " << scanDataMap["id"].toInt();
// qDebug() << "Fixed Number: " << scanDataMap["FixedNumber"].toUInt();
// qDebug() << "Timestamp: " << scanDataMap["Timestamp"].toUInt();
// qDebug() << "System Status: " << scanDataMap["SystemStatus"].toInt();
// qDebug() << "Total Pressure: " << scanDataMap["TotalPressure"].toDouble();
// qDebug() << "Baseline: " << scanDataMap["Baseline"].toDouble();
// qDebug() << "TPQuad: " << scanDataMap["TPQuad"].toDouble();
// qDebug() << "Total Pressure Detector Current: " << scanDataMap["TotalPressureDetectorCurrent"].toDouble();
// qDebug() << "Emission Current: " << scanDataMap["EmissionCurrent"].toInt();
// qDebug() << "Anode Potential: " << scanDataMap["AnodePotential"].toInt();
// qDebug() << "Electron Energy: " << scanDataMap["ElectronEnergy"].toInt();
// qDebug() << "Focus Potential: " << scanDataMap["FocusPotential"].toInt();
// qDebug() << "Filament Current: " << scanDataMap["FilamentCurrent"].toInt();

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

/*
readFloat32函数测试代码
*/
// QByteArray byteArray = QByteArray::fromHex("000128F3F952");

// // 假设我们需要从偏移量为0的位置提取一个浮动32位数值，并将其转换为科学计数法
// bool littleEndian = false;  // 设置字节序
// int offset = 2;  // 偏移量为2
// // 调用函数并接收返回值
// QPair<QString, float> result = BinaryConverter::readFloat32(byteArray,littleEndian, offset);

// double value = result.second;
// qDebug() << "value: " << value;
// // 输出科学计数法和浮动数值
// qDebug() << "Scientific notation:" << result.first;
// qDebug() << "Floating point value:" << result.second;
