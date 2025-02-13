#include "ScanMessageParser.h"
#include <QDebug>
#include <QList>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include <QBuffer>
#include <QtCore>
#include <qlist.h>


// ScanMessageParser类的构造函数，初始化scanSetupChannels成员变量
ScanMessageParser::ScanMessageParser(const QList<ScanSetupChannel>& ScanSetupChannels)
    : scanSetupChannels(ScanSetupChannels), nextPointIndex(0), isFirst(true) {}

ScanMessageParser::ScanMessageParser()
{
    nextPointIndex = 0;
    isFirst = true;
}
// 解析字节数组的方法
QList<ScanData> ScanMessageParser::parse(const QByteArray& byteArray) 
{
    // 如果字节数组为空，则直接返回一个空的QVariant对象
    if (byteArray.isEmpty()) {
        qDebug() << "parse - 字节数组为空。";
        return QList<ScanData>();
    }

    bool littleEndian = false; // 默认为大端序
    ScanData partialScan = {0};

    QList<ScanData> parsedScans;


    // 使用BinaryConverter::readUInt32方法读取前4个字节作为消息头
    quint32 header = BinaryConverter::readUInt32(byteArray, littleEndian, 0);  // 从偏移量0开始读取4个字节
    if (header != HEADER_MARKER) {
        qDebug() << "parse - 消息头与HEADER_MARKER不匹配，尝试大端格式读取。";  // 如果消息头与预期的常量不匹配，输出提示信息
    } else {
        qDebug() << "parse - 消息头匹配HEADER_MARKER。 通过";  // 如果消息头匹配预期的常量，输出成功信息
    }
    qDebug() << "header (消息头):" << header;  // 输出读取到的消息头

    // 读取 headerSize，偏移量为4，读取1个字节（表示头部的大小）
    quint8 headerSize = BinaryConverter::readUInt8(byteArray, littleEndian, 4); // 读取偏移量4处的一个字节
    qDebug() << "headerSize (头部大小):" << headerSize;  // 输出headerSize值

    // 读取 dataHeaderSize，偏移量为5，读取1个字节（表示数据头的大小）
    quint8 dataHeaderSize = BinaryConverter::readUInt8(byteArray, littleEndian, 5); // 读取偏移量5处的一个字节
    qDebug() << "dataHeaderSize (数据头大小):" << dataHeaderSize;  // 输出dataHeaderSize值

    // 读取 dataSize，偏移量为6，读取2个字节（表示数据的大小）
    quint16 dataSize = BinaryConverter::readUInt16(byteArray, littleEndian, 6); // 读取偏移量6处的两个字节
    qDebug() << "dataSize (数据大小):" << dataSize;  // 输出dataSize值

    // 读取 dataType，偏移量为8，读取2个字节（表示数据的类型）
    quint16 dataType = BinaryConverter::readUInt16(byteArray, littleEndian, 8); // 读取偏移量8处的两个字节
    qDebug() << "dataType (数据类型):" << dataType;  // 输出dataType值

    // 检查 dataType 是否匹配预期的BINARY_DATA_TYPE常量
    if (dataType != BINARY_DATA_TYPE) {
        qDebug() << "parse - 数据类型不匹配BINARY_DATA_TYPE。 不通过";  // 如果dataType与预期不匹配，输出错误信息
    } else {
        qDebug() << "parse - 数据类型匹配BINARY_DATA_TYPE。 通过";  // 如果dataType匹配预期值，输出成功信息
    }

    // 读取 reserved1，偏移量为10，读取2个字节（保留字段，通常用来对齐或未来扩展）
    quint16 reserved1 = BinaryConverter::readUInt16(byteArray, littleEndian, 10); 
    qDebug() << "reserved1 (保留字段1):" << reserved1;  // 输出reserved1的值

    // 读取 reserved2，偏移量为12，读取4个字节（另一个保留字段）
    quint32 reserved2 = BinaryConverter::readUInt32(byteArray, littleEndian, 12); 
    qDebug() << "reserved2 (保留字段2):" << reserved2;  // 输出reserved2的值

    // 读取 pointsSinceStartScan，偏移量为16，读取4个字节（表示从扫描开始到目前为止的点数）
    quint32 pointsSinceStartScan = BinaryConverter::readUInt32(byteArray, littleEndian, 16); 
    qDebug() << "pointsSinceStartScan (扫描开始到目前为止的点数):" << pointsSinceStartScan;  // 输出pointsSinceStartScan的值

    // 读取 scanSize，偏移量为20，读取4个字节（表示扫描的大小）
    quint32 scanSize = BinaryConverter::readUInt32(byteArray, littleEndian, 20); 
    qDebug() << "scanSize (扫描大小):" << scanSize;  // 输出scanSize的值

    // 读取 dataPointCount，偏移量为24，读取4个字节（表示数据点的数量）
    quint32 dataPointCount = BinaryConverter::readUInt32(byteArray, littleEndian, 24); 
    qDebug() << "dataPointCount (数据点数量):" << dataPointCount;  // 输出dataPointCount的值

    quint32 dataOffset = 28;  // 数据偏移量，从28开始


    // 计算 scanNum (表示当前扫描的编号,因为扫描不是从0开始的)
    // 计算当前扫描编号，使用 qFloor 来确保向下取整
    quint32 scanNum = qFloor(static_cast<qreal>(pointsSinceStartScan) / scanSize) + 1; 
    // 计算当前扫描的编号
    qDebug() << "scanNum (当前扫描编号):" << scanNum;  // 输出计算出来的scanNum

    // 初始化变量
    quint32 idx = 0;                // 索引变量，可能用于后续的数据处理
    bool ranOutOfPoints = false;    // 用于标记是否数据点已经用完
    quint32 pointCount = 0;         // 存储数据点的数量

    bool oneEnabled = false;        // 用于标记是否存在启用的通道

    // 遍历 ScanSetupChannels，检查是否有启用的通道
    for (int sscIdx = 0; sscIdx < scanSetupChannels.size(); ++sscIdx) {
        // 获取当前通道的 `enabled` 属性，并进行检查
        // `get("enabled")` 返回当前通道的 "enabled" 属性的值，`.toBool()` 将其转换为布尔值
        if (scanSetupChannels[sscIdx].get("enabled").toBool() == true) {
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
    if (isFirst && pointsSinceStartScan) {
        // 我们需要与最新的扫描对齐，因此需要确定当前的位置
        // 然后改变偏移量，以便这些点不会被处理
        quint32 pointsToPurge = scanSize - (pointsSinceStartScan % scanSize);  // 计算需要跳过的点数
        dataOffset += pointsToPurge * DATA_POINT_SIZE;  // 更新数据偏移量，跳过这些点
        pointCount += pointsToPurge;  // 增加点数计数，跳过的点数也要算进去
        scanNum += 1;  // 当前扫描编号加1，表示已经完成了一轮扫描
    }

    // 设置 isFirst 为 false，表示第一次处理已经结束
    isFirst = false;



    // 当没有耗尽数据点并且扫描设置通道不为空时，继续处理
    while (!ranOutOfPoints && scanSetupChannels.size()) 
    {
        // 遍历所有扫描设置通道
        for (int channelIdx = 0; channelIdx < scanSetupChannels.size() && !ranOutOfPoints; ++channelIdx) 
        {
            // 获取当前通道
            const ScanSetupChannel &channel = scanSetupChannels[channelIdx];

            // 如果当前通道未启用，则跳过该通道
            if (!channel.get("enabled").toBool()) {
                continue;
            }

            // 获取当前通道的模式
            QString channelMode = channel.get("channelMode").toString();

            // 如果通道模式是 "Sweep"，处理该通道数据
            if(channelMode == "Sweep")
            {
                // 计算当前通道总点数
                int totalChannelPoints = static_cast<int>(
                        (channel.get("StopMass").toDouble() - channel.get("StartMass").toDouble()) * channel.get("Ppamu").toDouble() + 1);

                // 遍历每个数据点，读取数据并存入缓冲区
                for (int i = 0; i < totalChannelPoints; ++i) 
                {
                    // 如果索引超出缓冲区大小
                    if (idx >= buffer.size()) 
                    {
                        // 检查是否已经达到数据字节数组的结尾
                        if (dataOffset + DATA_POINT_SIZE > byteArray.size()) 
                        { 
                            ranOutOfPoints = true; // 如果数据点不足，标记为超出点数
                            break; // 退出点数循环
                        } 
                        else
                        {
                            // 否则，读取数据并增加数据偏移量
                            buffer.push_back(BinaryConverter::readFloat32(byteArray, littleEndian, dataOffset).second); // 读取浮动32位数据（即 double 类型数据）
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
                }
                idx++;  // 增加当前索引
                break;  // 跳出当前循环，进入下一个通道处理
            }
            // 其他类型的通道处理
            else 
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
                        // 如果通道类型为 F32Value，读取浮动32位数据；否则读取无符号32位数据
                        if(channel.get("channelType").toString() == "F32Value")
                        {
                            buffer.push_back(BinaryConverter::readFloat32(byteArray, littleEndian, dataOffset).second);
                        }
                        else 
                        {
                            buffer.push_back(BinaryConverter::readUInt32(byteArray, littleEndian, dataOffset));
                        }
                        dataOffset += DATA_POINT_SIZE;  // 增加数据偏移量
                        pointCount++;  // 增加数据点计数
                    }
                }
                idx++; // 增加当前索引
            }
        }

        // 根据当前的扫描数据创建扫描属性
        ScanData scanAttributes = toAttributes(scanNum, scanSize, buffer);

        // 如果数据点不足
        if (ranOutOfPoints) 
        {
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
            // 检查缓冲区的长度是否与扫描大小匹配
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
            buffer.clear();  
        }
    }


    qDebug() << "parse - 解析方法结束。";
    // 返回一个空的QVariant对象（实际上这里应该包含解析后的数据）
    return parsedScans;
}

// 获取下一个数据点的索引
int ScanMessageParser::getNextPointIndex() const
{
    qDebug() << "getNextPointIndex - 返回nextPointIndex:" << nextPointIndex;
    return nextPointIndex;
}

// 将扫描数据转换为属性格式的方法
ScanData ScanMessageParser::toAttributes(int scanNum, int scanSize, const QList<double> buffer) const 
{
    int idx = 0;
    Measurement measurement;  // 存储每个通道的测量数据
    // 创建 ScanData 对象
    ScanData scanData;
    scanData.id = scanNum;
    scanData.scanNum = scanNum;
    scanData.scanSize = scanSize;


    // 遍历每个扫描设置通道
    for (const auto& channel : scanSetupChannels) 
    {
        // 只有在通道启用且有数据时才处理
        if (!channel.get("enabled").toBool() || idx >= buffer.size()) {
            qDebug() << "toAttributes - 通道未启用或索引超出范围，跳过该通道。";
            continue;
        }

        // 根据通道模式计算需要提取的数据点数量
        QString channelMode = channel.get("channelMode").toString();
        if (channelMode == "Sweep") 
        {    
            // 计算当前通道需要提取的数据点总数
            int channelTotalPoints = static_cast<int>(((channel.get("stopMass").toDouble() - channel.get("startMass").toDouble()) * channel.get("ppamu").toDouble()) + 1);
            measurement.channelId = channel.get("id").toInt();
            measurement.stopMass = channel.get("stopMass").toDouble();
            measurement.startMass = channel.get("startMass").toDouble();
            measurement.ppamu = channel.get("ppamu").toDouble();
            measurement.data = buffer.mid(idx, channelTotalPoints);
            idx += channelTotalPoints;
        }
        // 处理SINGLE模式的通道
        else if (channelMode == "Single")
        {
            measurement.channelId = channel.get("id").toInt();
            measurement.stopMass = channel.get("stopMass").toDouble();
            measurement.startMass = channel.get("startMass").toDouble();
            measurement.ppamu = channel.get("ppamu").toDouble();
            measurement.data.append(buffer[idx]);
            idx += 1;
        }
        // 处理其他模式的通道
        else
        {
            if(channelMode == "FixedNumber")
            {
                scanData.fixedNumber = static_cast<quint32>(buffer[idx]);
                idx += 1;
            }
            else if(channelMode == "Timestamp")
            {
                scanData.timestamp = static_cast<quint64>(buffer[idx]);
                idx += 1;
            }
            else if(channelMode == "SystemStatus")
            {
                scanData.systemStatus = static_cast<quint32>(buffer[idx]);
                idx += 1;
            }
            else if(channelMode == "TotalPressure")
            {
                scanData.totalPressure = buffer[idx];
                idx += 1;
            }
            else if(channelMode == "Baseline")
            {
                scanData.baseline = buffer[idx];
                idx += 1;
            }
            else if(channelMode == "TPQuad")
            {
                scanData.tpQuad = buffer[idx];
                idx += 1;
            }
            else if(channelMode == "TotalPressureDetectorCurrent")
            {
                scanData.totalPressureDetectorCurrent = buffer[idx];
                idx += 1;
            }
            else if(channelMode == "EmissionCurrent")
            {
                scanData.emissionCurrent = buffer[idx];
                idx += 1;
            }
            else if(channelMode == "AnodePotential")
            {
                scanData.anodePotential = buffer[idx];
                idx += 1;
            }
            else if(channelMode == "ElectronEnergy")
            {
                scanData.electronEnergy = buffer[idx];
                idx += 1;
            }
            else if(channelMode == "FocusPotential")
            {
                scanData.focusPotential = buffer[idx];
                idx += 1;
            }
            else if(channelMode == "FilamentCurrent")
            {
                scanData.filamentCurrent = buffer[idx]; 
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
QVariantMap ScanMessageParser::convertScanDataToMap(const ScanData &scanData) 
{
    QVariantMap scanDataMap;
    scanDataMap["scanNum"] = scanData.scanNum;
    scanDataMap["scanSize"] = scanData.scanSize;
    scanDataMap["id"] = scanData.id;
    scanDataMap["FixedNumber"] = scanData.fixedNumber;
    scanDataMap["Timestamp"] = scanData.timestamp;
    scanDataMap["SystemStatus"] = scanData.systemStatus;
    scanDataMap["TotalPressure"] = scanData.totalPressure;
    scanDataMap["Baseline"] = scanData.baseline;
    scanDataMap["TPQuad"] = scanData.tpQuad;
    scanDataMap["TotalPressureDetectorCurrent"] = scanData.totalPressureDetectorCurrent;
    scanDataMap["EmissionCurrent"] = scanData.emissionCurrent;
    scanDataMap["AnodePotential"] = scanData.anodePotential;
    scanDataMap["ElectronEnergy"] = scanData.electronEnergy;
    scanDataMap["FocusPotential"] = scanData.focusPotential;
    scanDataMap["FilamentCurrent"] = scanData.filamentCurrent;

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

void ScanMessageParser::printScanData(const QVariantMap &scanDataMap) 
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

    // 打印 measurements
    QVariantList measurementsList = scanDataMap["measurements"].toList();
    for (const QVariant &measurementVariant : measurementsList) {
        QVariantMap measurementMap = measurementVariant.toMap();

        int channelId = measurementMap["channelId"].toInt();
        double stopMass = measurementMap["stopMass"].toDouble();
        double startMass = measurementMap["startMass"].toDouble();
        double ppamu = measurementMap["ppamu"].toDouble();
        QList<double> data = measurementMap["data"].value<QList<double>>();

        // 打印每个测量数据项
        qDebug() << "Measurement Info:";
        qDebug() << "  ChannelId: " << channelId;
        qDebug() << "  StopMass: " << stopMass;
        qDebug() << "  StartMass: " << startMass;
        qDebug() << "  Ppamu: " << ppamu;
        qDebug() << "  Data: ";
        for (double value : data) {
            qDebug() << "    " << value;
        }
    }
}

QVariant ScanMessageParser::getJsonField(const QString &jsonStr, const QString &field) 
{
    // 将 JSON 字符串解析为 QJsonDocument 对象
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    if (!doc.isObject()) {
        return QVariant();  // 如果解析失败，返回无效的 QVariant
    }

    QJsonObject jsonObj = doc.object();
    return getJsonField(jsonObj, field);  // 调用递归版本的函数
}

QVariant ScanMessageParser::getJsonField(const QJsonObject &jsonObj, const QString &field) 
{
    // 如果目标字段在当前对象中存在，直接返回
    if (jsonObj.contains(field)) {
        return jsonObj[field].toVariant();  // 返回目标字段的值
    }

    // 如果目标字段在当前对象中不存在，递归查找子对象或数组
    for (auto key : jsonObj.keys()) {
        if (jsonObj[key].isObject()) {
            QVariant result = getJsonField(jsonObj[key].toObject(), field);  // 递归查找子对象
            if (result.isValid()) {
                return result;  // 找到字段，返回值
            }
        }
        if (jsonObj[key].isArray()) {
            for (const auto &item : jsonObj[key].toArray()) {
                if (item.isObject()) {
                    QVariant result = getJsonField(item.toObject(), field);  // 递归查找数组元素中的对象
                    if (result.isValid()) {
                        return result;
                    }
                }
            }
        }
    }

    // 如果没有找到目标字段，返回无效的 QVariant
    return QVariant();
}


// 获取 scanSetupChannels 变量（线程安全）
QList<ScanSetupChannel> ScanMessageParser::getScanSetupChannels() const
{
    // 使用 QMutexLocker 自动加锁和解锁
    QMutexLocker locker(&mutex);
    return scanSetupChannels;
}

// 设置 scanSetupChannels 变量（线程安全）
void ScanMessageParser::setScanSetupChannels(const QList<ScanSetupChannel>& channels)
{
    // 使用 QMutexLocker 自动加锁和解锁
    QMutexLocker locker(&mutex);
    scanSetupChannels = channels;
}
