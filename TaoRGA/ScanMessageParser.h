#ifndef SCANMESSAGEPARSER_H
#define SCANMESSAGEPARSER_H

#include <QList>
#include <QVariant>
#include "ScanSetupChannel.h"
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QtEndian>
#include <cmath>
#include <QPair>
#include <QMutex>
#include <QMutexLocker>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


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
        qDebug() << "Extracted bytes:" << tempArray.toHex();
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
        qDebug() << "Raw data (hex):" << QString::number(rawData, 16);
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
        qDebug() << "Read float value:" << result;
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
    int scanNum;    // 扫描编号
    int scanSize;   // 扫描大小
    int id;         // ID
    quint32 fixedNumber;  // 固定数字
    quint32 timestamp;    // 时间戳
    int systemStatus;     // 系统状态
    double totalPressure; // 总压力
    double baseline;      // 基线
    double tpQuad;        // 四分之一压力
    double totalPressureDetectorCurrent;  // 总压力探测器电流
    int emissionCurrent;  // 发射电流
    int anodePotential;   // 阳极电位
    int electronEnergy;   // 电子能量
    int focusPotential;   // 聚焦电位
    int filamentCurrent;  // 灯丝电流
    QList<Measurement> measurements; // 测量数据
};



class ScanMessageParser : public QObject
{
    Q_OBJECT
public:
    // 默认构造函数
    ScanMessageParser();
    
    // 带参数构造函数
    ScanMessageParser(const QList<ScanSetupChannel>& scanSetupChannels);

    // 解析二进制数据
    QList<ScanData> parse(const QByteArray& byteArray);
    
    // 获取 JSON 字符串中的字段值
    QVariant getJsonField(const QString &jsonStr, const QString &field); 
    QVariant getJsonField(const QJsonObject &jsonObj, const QString &field); 

    
    // 获取下一个数据点的索引
    int getNextPointIndex() const;

    // 封装的一个函数，用于将 ScanData 转换为 QVariantMap
    QVariantMap convertScanDataToMap(const ScanData &scanData);
    
    // 打印一个 ScanData 对象
    void printScanData(const QVariantMap &scanDataMap);
    
    // 获取 scanSetupChannels 变量（线程安全）
    QList<ScanSetupChannel> getScanSetupChannels() const;
    
    // 设置 scanSetupChannels 变量（线程安全）
    void setScanSetupChannels(const QList<ScanSetupChannel>& channels);


private:
    // 当前通道的配置
    QList<ScanSetupChannel> scanSetupChannels;

    // 线程互斥锁，确保访问 scanSetupChannels 变量的线程安全
    mutable QMutex mutex;

    // 表示第一次扫描
    bool isFirst = true;
    
    // 表示下一个数据点的索引
    int nextPointIndex = 0;
    
    // 扫描数据缓冲区
    QList<double> buffer;

    // 将扫描数据转换为属性的函数
    ScanData toAttributes(int scanNum, int scanSize, const QList<double> buffer) const;
};



#endif // SCANMESSAGEPARSER_H

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
