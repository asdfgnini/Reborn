#ifndef CUSTOMFILE_H
#define CUSTOMFILE_H

#include <QObject>
#include <QString>
#include <QFile>

class CustomFile : public QObject
{
    Q_OBJECT

public:
    // 构造函数
    explicit CustomFile(QObject *parent = nullptr);

    // 写入文件的函数
    bool createAndWriteToFile(int dwell, int ppamu, const QString &addr, int dataCount);

    // 关闭文件
    void closeFile();
    // 写入数据
    bool writeDataToFile(double scanNum, double scanTm, double totalPressure,QList<double> &dataList);

    // 检查文件是否已创建
    bool isFileCreated() const;

private:
    bool m_fileCreated = false;  // 文件是否已创建
    QFile m_file;                // 文件对象
};


#endif // CUSTOMFILE_H
