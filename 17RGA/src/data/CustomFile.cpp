#include "../include/CustomFile.h"
#include <QTextStream>
#include <QDateTime>
#include <QCoreApplication>
#include <QDir>

CustomFile::CustomFile(QObject *parent)
    : QObject(parent),m_fileCreated(false)
{

}

bool CustomFile::createAndWriteToFile(int dwell, int ppamu, const QString &addr, int dataCount)
{
    // 判断文件是否已创建
    if (m_fileCreated)
    {
        return false; // 文件已创建并写入
    }

    // 获取当前可执行文件的路径
    QString appPath = QCoreApplication::applicationDirPath();

    // 创建 data 目录的完整路径
    QString dataDirPath = appPath + "/data";

    // 创建目录（如果目录不存在）
    QDir dir;
    if (!dir.exists(dataDirPath)) {
        dir.mkpath(dataDirPath);  // mkpath 会创建多层目录
    }

    // 自动生成文件名，例如使用当前时间戳和指定的格式
    QString fileName = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + "_myRGA_myRGA.csv";

    // 设置文件名并创建完整的文件路径
    QString filePath = dataDirPath + "/" + fileName;
    m_file.setFileName(filePath);

    // 打开文件进行写入
    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false; // 文件打开失败
    }

    QTextStream out(&m_file);

    // 获取当前时间并格式化为 StartTm
    QString startTm = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // 按照csv格式写入数据，使用逗号作为分隔符
    out << "Dwell," << dwell << "\n";
    out << "PPamu," << ppamu << "\n";
    out << "Addr," << addr << "\n";
    out << "StartTm," << startTm << "\n";

    // 写入表头：ScanNum, ScanTm, TotalPressure, data0, data1, ..., dataN
    out << "ScanNum," << "ScanTm," << "TotalPressure,";
    out << "data,";
    for (int i = 0; i <= dataCount; ++i)
    {
        out << i;
        if (i < dataCount+1)
        {
            out << ",";
        }
    }
    out << "\n";  // 换行，结束表头

    // 标记文件已创建
    m_fileCreated = true;
    return true;
}


bool CustomFile::writeDataToFile(double scanNum, double scanTm, double totalPressure, QList<double> &dataList)
{
    if (!m_fileCreated)
    {
        return false; // 文件没有创建，无法写入
    }

    QTextStream out(&m_file);

    // 插入前置数据 ScanNum, ScanTm, TotalPressure 并在后面添加一个空格
    out << QString::number(scanNum) << ",";  // scanNum 以常规格式写入
    out << QString::number(scanTm,'f',0) << ",";  // scanTm 以浮动格式（'f'）写入，保留 6 位小数
    out << QString::number(totalPressure) << ", ,";  // totalPressure 以浮动格式（'f'）写入，保留 6 位小数

    // 向文件中写入数据，确保每个 double 类型的数字用逗号分隔
    for (int i = 0; i < dataList.size(); ++i)
    {
        // 使用科学计数法格式写入每个数据
        out << QString::number(dataList[i], 'e', 2);  // 'e'表示科学计数法，2表示保留两位小数
        if (i < dataList.size() - 1) {  // 如果不是最后一个元素
            out << ",";  // 用逗号分隔
        }
    }
    out << "\n";  // 换行

    return true;
}


void CustomFile::closeFile()
{
    if (m_fileCreated)
    {
        m_file.close();
        m_fileCreated = false;  // 结束文件写入
    }
}

bool CustomFile::isFileCreated() const
{
    return m_fileCreated;
}


