#ifndef T_SCANMODE_H
#define T_SCANMODE_H

#include "T_ScanModeBasePage.h"
#include "CustomPlot.h"
#include "ElaMultiSelectComboBox.h"

struct ProcessedMeasurements {
    double tic;  // 总离子计数
    QMap<double, double> wholeMassValues;  // 整数质量值对应的数据
};

class T_ScanMode : public T_ScanBasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_ScanMode(QWidget* parent = nullptr);
    ~T_ScanMode();
    // 初始化所有图形
    void ui_init();
  // 初始化所有图形
    void setGraphDataSource(int index, const QString& dataType);
    void addGraph(CustomPlot* graph);
    QString convertTimestamp(qint64 timestamp);
    void testTimeConversion(); // 可选的测试函数

public slots:
    // 添加要显示的属性
    void addPropertyToSingleGraph(const QString& propertyName);
    // 移除要显示的属性
    void removePropertyFromSingleGraph(const QString& propertyName);
    // 清除所有属性
    void clearPropertiesFromSingleGraph();
    // 处理新数据到达的槽
    void onNewDataAvailable(const QVariantMap& data);
    void onconnectisready(bool isScanning,int startMass,int stopMass,int ppamu,int dwell);
    void onconnectclickedclose();
    void onscanclicked(bool isready);
    void onscanclickedclose(bool iscan);
    // 单位类型改变槽函数，用于接收后端返回的单位类型乘数，然后更新单位类型乘数
    void onunitMultiplierChanged(double unitMultiplier);
    void clearAllGraphData();
    void ontotalPressureChanged(double totalPressure);
    void onscanmodeconnectclicked(QString ip, int port,bool checked);
    void onscanmodeclicked(QString startMass,QString stopMass,QString ppamu,QString dwell,bool scanstate);
    void onscanmodeconnectclickedclose(bool checked);
    void onscanmodeclickedclose(bool checked);
signals:
    void unitMultiplierChanged(QString unitType);
    void scanmodeconnectclicked(QString ip, int port,bool checked);
    void scanmodeclicked(QString startMass,QString stopMass,QString ppamu,QString dwell,bool scanstate);
    void scanmodeconnectclickedclose(bool checked);
    void scanmodeclickedclose(bool checked);
private:
    ProcessedMeasurements processMeasurements(const QVariantList& measurements);
    // 添加新的辅助函数来更新图表
    void updateGraphsWithNewUnit();
    void initScanGraph(CustomPlot* graph);

    void initSingleGraph(CustomPlot* graph);
    void drawScanGraph(CustomPlot* graph, const QVector<double>& x, const QVector<double>& y, 
                      const QVector<double>& lastX, const QVector<double>& lastY);
    void drawSingleGraph(CustomPlot* graph, const QVector<double>& x, 
                        const QMap<QString, QVector<double>>& propertyData);
    void testDrawGraphs();

private:
    double m_totalPressure = 1.0;  // 默认值为1.0
    QCPItemText* m_tracerLabel = nullptr;
    // scan模式的数据结构
    struct ScanGraphInfo {
        CustomPlot* graph;
        QVector<double> xData;      // 当前数据的x值
        QVector<double> yData;      // 当前数据的y值
        QVector<double> lastXData;  // 上一轮数据的x值
        QVector<double> lastYData;  // 上一轮数据的y值
        QList<double> rawData;  // 添加存储原始数据的成员
        QList<double> lastRawData; // 上一轮原始数据
    };

    // single模式的数据结构
    struct SingleGraphInfo {
        CustomPlot* graph;
        QVector<double> xData;      // 时间轴数据
        QSet<QString> propertyNames;  // 当前显示的属性集合
        QMap<QString, QVector<double>> propertyData;  // 每个属性的数据
    };

    // 统一的图表信息结构
    struct GraphInfo {
        CustomPlot* graph;
        QString dataType;  // "scan" 或 "single"
        union {
            ScanGraphInfo* scanInfo;
            SingleGraphInfo* singleInfo;
        };
    };

    // 属性配置结构
    struct PropertyConfig {
        QString displayName;  // 显示名称
        QColor color;        // 曲线颜色
        QString unit;        // 单位
        double scaleFactor;  // 添加一个缩放因子
    };

    QList<GraphInfo> m_graphInfos;  // 存储所有图形的列表
    QMap<QString, PropertyConfig> m_availableProperties;  // 存储所有可用属性的配置

    // 单位类型,用于坐标转换
    ElaComboBox* unitType;
    //属性选择
    ElaMultiSelectComboBox* _multiSelectComboBox;
    // 单位类型乘数,用于坐标转换,一开始默认时Amps,此时该系数为1
    double newUnitMultiplier = 1;
    double oldUnitMultiplier = 1;
    

};

// // 在T_ScanMode中添加模式切换按钮
// void T_ScanMode::onModeButtonClicked()
// {
//     // 通过父设备切换到漏检模式
//     RGADevice* device = qobject_cast<RGADevice*>(parent());
//     if (device) {
//         device->switchToMode("LeakMode");
//     }
// }

#endif // T_BASECOMPONENTS_H
