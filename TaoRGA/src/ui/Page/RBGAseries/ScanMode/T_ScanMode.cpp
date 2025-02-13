#include "T_ScanMode.h"

#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <qnamespace.h>
#include "ElaScrollPageArea.h"
#include "ElaToggleSwitch.h"
#include "ElaText.h"
#include "ElaMultiSelectComboBox.h"
#include "ElaMessageBar.h"

T_ScanMode::T_ScanMode(QWidget* parent)
    : T_ScanBasePage(parent)
{

    //做个拦截
    connect(this,&T_ScanMode::connectclicked,this,&T_ScanMode::onscanmodeconnectclicked);
    connect(this,&T_ScanMode::saomiaoclicked,this,&T_ScanMode::onscanmodeclicked);
    connect(this,&T_ScanMode::connectclickedclose,this,&T_ScanMode::onscanmodeconnectclickedclose);
    connect(this,&T_ScanMode::saomiaoclickedclose,this,&T_ScanMode::onscanmodeclickedclose);
    // QTimer::singleShot(1000, this, &T_ScanMode::testDrawGraphs);

    m_availableProperties = {
        {"TotalPressure", {
            "总压",
            Qt::blue,
            "Torr",
            1.0
        }},
        {"Baseline", {
            "基线",
            Qt::red,
            "",
            1.0
        }},
        {"TPQuad", {
            "四极杆总压",
            Qt::green,
            "",
            1.0
        }},
        {"EmissionCurrent", {
            "发射电流",
            Qt::cyan,
            "μA",
            1.0
        }},       
        {"AnodePotential", {
            "阳极电位",
            Qt::yellow,
            "V",
            0.01
        }},
        {"ElectronEnergy", {
            "电子能量",
            Qt::darkGreen,
            "V",
            0.01
        }},
        {"FocusPotential", {
            "聚焦电位",
            Qt::darkBlue,
            "V",
            0.01
        }},
        {"FilamentCurrent", {
            "灯丝电流",
            Qt::darkRed,
            "A",
            0.001
        }}
    };



    ui_init();


}

void T_ScanMode::ui_init()
{
    // 预览窗口标题
    setWindowTitle("ElaBaseComponents");

    // 顶部元素
    createCustomWidget("下面用于RGA数据的可视化展示");

    //显示区域
    ElaText* toggleSwitchDisableText = new ElaText("扫描离子流", this);
    toggleSwitchDisableText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    toggleSwitchDisableText->setTextPixelSize(15);
    ElaText* toggleSwitchDisableText2 = new ElaText("电子倍增器", this);
    toggleSwitchDisableText2->setTextInteractionFlags(Qt::TextSelectableByMouse);
    toggleSwitchDisableText2->setTextPixelSize(15);    
    ElaToggleSwitch* toggleSwitchDisableSwitch = new ElaToggleSwitch(this);    
    ElaText* toggleSwitchDisableText3 = new ElaText("坐标转换", this);

    toggleSwitchDisableText3->setTextInteractionFlags(Qt::TextSelectableByMouse);
    toggleSwitchDisableText3->setTextPixelSize(15);
    QStringList multiComboList1{"AMPs", "Torr", "mB", "Pa", "PPM"};
    unitType = new ElaComboBox(this);
    unitType->addItems(multiComboList1);
    unitType->setCurrentIndex(0);
    connect(unitType, &ElaComboBox::currentIndexChanged, this, [this,multiComboList1](int index) {

        // 我们在这里需要计算出单位类型乘数,用于坐标转换,一开始默认时Amps,此时该系数为1
        //计算单位类型乘数需要三个变量Sensitivity，FragFactor，IonizationProbability
        // 我们发出信号，让后端计算后返回计算完的unitMultiplier，如果是Amps，则不需要发出信号
        if(index == 0)//Amps
        {        

            oldUnitMultiplier = newUnitMultiplier;
            newUnitMultiplier = 1;
            qDebug() << "单位类型:" << multiComboList1[index];
            qDebug() << "oldUnitMultiplier:" << oldUnitMultiplier;
            qDebug() << "newUnitMultiplier:" << newUnitMultiplier;

            // 重新计算并更新当前轮数据
            updateGraphsWithNewUnit();
        }
        else if(index == 1)//Torr
        {
           qDebug() << "单位类型:" << multiComboList1[1];
           qDebug() << "oldUnitMultiplier:" << oldUnitMultiplier;
           qDebug() << "newUnitMultiplier:" << newUnitMultiplier;
           oldUnitMultiplier = newUnitMultiplier;
           emit unitMultiplierChanged(multiComboList1[1]);
        }
        else if(index == 2)//mB
        {
            qDebug() << "单位类型:" << multiComboList1[2];
            qDebug() << "oldUnitMultiplier:" << oldUnitMultiplier;
            qDebug() << "newUnitMultiplier:" << newUnitMultiplier;
            oldUnitMultiplier = newUnitMultiplier;
            emit unitMultiplierChanged(multiComboList1[2]);
        }
        else if(index == 3)//Pa
        {
            qDebug() << "单位类型:" << multiComboList1[3];
            qDebug() << "oldUnitMultiplier:" << oldUnitMultiplier;
            qDebug() << "newUnitMultiplier:" << newUnitMultiplier;
            oldUnitMultiplier = newUnitMultiplier;
            emit unitMultiplierChanged(multiComboList1[3]);
        }
        else if(index == 4)//PPM
        {
            qDebug() << "单位类型:" << multiComboList1[4];
            qDebug() << "oldUnitMultiplier:" << oldUnitMultiplier;
            qDebug() << "newUnitMultiplier:" << newUnitMultiplier;
            oldUnitMultiplier = newUnitMultiplier;
            emit unitMultiplierChanged(multiComboList1[4]);
        }
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(toggleSwitchDisableText);
    buttonLayout->addStretch();
    buttonLayout->addWidget(toggleSwitchDisableText2);
    buttonLayout->addSpacing(2);
    buttonLayout->addWidget(toggleSwitchDisableSwitch);
    buttonLayout->addStretch();
    buttonLayout->addWidget(toggleSwitchDisableText3);
    buttonLayout->addSpacing(2);
    buttonLayout->addWidget(unitType);
    buttonLayout->addStretch();

    //第一个图表
    CustomPlot* scanGraph = new CustomPlot(this);
    addGraph(scanGraph);
    setGraphDataSource(0, "scan");  // 设置为扫描数据类型
    scanGraph->setFixedHeight(270);   
    initScanGraph(scanGraph);
 


    //显示区域
    ElaText* toggleSwitchDisableText12 = new ElaText("单点离子流", this);
    toggleSwitchDisableText12->setTextInteractionFlags(Qt::TextSelectableByMouse);
    toggleSwitchDisableText12->setTextPixelSize(15);
    ElaText* toggleSwitchDisableText22 = new ElaText("其他操作", this);
    toggleSwitchDisableText22->setTextInteractionFlags(Qt::TextSelectableByMouse);
    toggleSwitchDisableText22->setTextPixelSize(15);    
    ElaText* toggleSwitchDisableText32 = new ElaText("坐标转换", this);
    toggleSwitchDisableText32->setTextInteractionFlags(Qt::TextSelectableByMouse);
    toggleSwitchDisableText32->setTextPixelSize(15);

    toggleSwitchDisableText22->setDisabled(true);
    toggleSwitchDisableText22->setVisible(false);
    toggleSwitchDisableText32->setDisabled(true);
    toggleSwitchDisableText32->setVisible(false);
    
    ElaToggleSwitch* toggleSwitchDisableSwitch12 = new ElaToggleSwitch(this);
    ElaToggleSwitch* toggleSwitchDisableSwitch22 = new ElaToggleSwitch(this);
    toggleSwitchDisableSwitch12->setDisabled(true);
    toggleSwitchDisableSwitch12->setVisible(false);
    toggleSwitchDisableSwitch22->setDisabled(true);
    toggleSwitchDisableSwitch22->setVisible(false);
    // connect(toggleSwitchDisableSwitch22, &ElaToggleSwitch::toggled, [this](bool checked) {
    //     if (checked) 
    //     {
    //         qDebug() << "添加属性";
    //         addPropertyToSingleGraph("TotalPressure");
    //     } else {
    //         qDebug() << "移除属性";
    //         removePropertyFromSingleGraph("TotalPressure");
    //     }

    // });

    ElaText* toggleSwitchDisableText42 = new ElaText("添加属性", this);
    toggleSwitchDisableText42->setTextInteractionFlags(Qt::TextSelectableByMouse);
    toggleSwitchDisableText42->setTextPixelSize(15);

    QStringList multiComboList{"总压信号Total Pressure", "基线信号Baseline", "压力信号 TP Quad", "发射电流Emission Current", "阳极电位Anode Potential", "电子能量Electron Energy", "聚焦电位Focus Potential", "灯丝电流Filament Current"};
    QStringList multiComboListchild{"TotalPressure", "Baseline", "TPQuad", "EmissionCurrent", "AnodePotential", "ElectronEnergy", "FocusPotential", "FilamentCurrent"};
    _multiSelectComboBox = new ElaMultiSelectComboBox(this);
    _multiSelectComboBox->addItems(multiComboList);
    //设置默认选中，谁都不选
    _multiSelectComboBox->setCurrentSelection(QStringList());
    connect(_multiSelectComboBox,&ElaMultiSelectComboBox::itemSelectionChanged,this,[this,multiComboListchild](QVector<bool> itemSelection)
    {
        for(int i = 0; i < itemSelection.size(); i++)
        {
            if(itemSelection[i])
            {
                addPropertyToSingleGraph(multiComboListchild[i]);
            }
            else
            {
                removePropertyFromSingleGraph(multiComboListchild[i]);
            }
        }

    
    }); 

    QHBoxLayout* buttonLayout2 = new QHBoxLayout();
    buttonLayout2->addWidget(toggleSwitchDisableText12);
    buttonLayout2->addStretch();
    buttonLayout2->addWidget(toggleSwitchDisableText22);
    buttonLayout2->addSpacing(2);
    buttonLayout2->addWidget(toggleSwitchDisableSwitch12);
    buttonLayout2->addStretch();
    buttonLayout2->addWidget(toggleSwitchDisableText32);
    buttonLayout2->addSpacing(2);
    buttonLayout2->addWidget(toggleSwitchDisableSwitch22);
    buttonLayout2->addStretch();
    buttonLayout2->addWidget(toggleSwitchDisableText42);
    buttonLayout2->addSpacing(2);
    buttonLayout2->addWidget(_multiSelectComboBox);
    buttonLayout2->addStretch();

    //第二个图表
    CustomPlot* singleGraph = new CustomPlot(this);
    addGraph(singleGraph);
    setGraphDataSource(1, "single");  // 设置为单点数据类型
    singleGraph->setFixedHeight(270);  
    initSingleGraph(singleGraph);


    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("RGA扫描模式");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addLayout(buttonLayout);
    centerLayout->addWidget(scanGraph);

    centerLayout->addLayout(buttonLayout2);
    centerLayout->addWidget(singleGraph);

    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);


}

T_ScanMode::~T_ScanMode()
{
    // 清理每个图表的数据结构
    for (GraphInfo& info : m_graphInfos) {
        if (info.dataType == "scan" && info.scanInfo) {
            delete info.scanInfo;
            info.scanInfo = nullptr;
        } 
        else if (info.dataType == "single" && info.singleInfo) {
            delete info.singleInfo;
            info.singleInfo = nullptr;
        }
    }
    m_graphInfos.clear();
    //清理所有items
    for (GraphInfo& info : m_graphInfos) {
        info.graph->clearItems();
    }
}


void T_ScanMode::setGraphDataSource(int index, const QString& dataType)
{
    if (index >= 0 && index < m_graphInfos.size()) {
        GraphInfo& info = m_graphInfos[index];
        info.dataType = dataType;
        
        // 根据类型创建对应的数据结构
        if (dataType == "scan") {
            info.scanInfo = new ScanGraphInfo();
            info.scanInfo->graph = info.graph;
        } else if (dataType == "single") {
            info.singleInfo = new SingleGraphInfo();
            info.singleInfo->graph = info.graph;
        }
    }
}
void T_ScanMode::addGraph(CustomPlot* graph)
{
    GraphInfo info;
    info.graph = graph;
    m_graphInfos.append(info);
}



void T_ScanMode::initScanGraph(CustomPlot* graph)
{
    if (!graph) return;

    // 添加标题
    graph->plotLayout()->insertRow(0);
    graph->plotLayout()->addElement(0, 0, new QCPTextElement(graph, "扫描离子流", QFont("黑体", 12, QFont::Bold)));

    // 设置图例
    graph->legend->setVisible(true);
    QFont legendFont = graph->font();
    legendFont.setPointSize(9);
    graph->legend->setFont(legendFont);
    graph->legend->setBrush(QBrush(QColor(255,255,255,230)));
    graph->legend->setSelectableParts(QCPLegend::spItems);
    graph->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    // 设置坐标轴
    graph->xAxis->setLabel("质量数 (amu)");
    graph->yAxis->setLabel("离子流 (A)");

    // 设置坐标轴范围自动同步
    connect(graph->xAxis, SIGNAL(rangeChanged(QCPRange)), graph->xAxis2, SLOT(setRange(QCPRange)));
    connect(graph->yAxis, SIGNAL(rangeChanged(QCPRange)), graph->yAxis2, SLOT(setRange(QCPRange)));

    // 启用交互
    graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // 添加两条曲线
    graph->addGraph();  // 当前数据曲线
    graph->graph(0)->setName("当前数据");
    graph->graph(0)->setPen(QPen(Qt::red, 2));

    graph->addGraph();  // 上一轮数据曲线
    graph->graph(1)->setName("上一轮数据");
    graph->graph(1)->setPen(QPen(Qt::blue, 1));
    graph->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    graph->rescaleAxes(true);

    // 创建一个追踪器标签
    QCPItemText* tracerLabel = new QCPItemText(graph);
    tracerLabel->setLayer("overlay");
    tracerLabel->setVisible(false);
    tracerLabel->setPadding(QMargins(5, 5, 5, 5));
    tracerLabel->setBrush(QBrush(QColor(255, 255, 255, 230)));
    tracerLabel->setPen(QPen(Qt::black));

    connect(graph, &CustomPlot::mouseMove, this, [=](QMouseEvent* event) {
        // 找到对应的 GraphInfo
        GraphInfo* graphInfo = nullptr;
        for (GraphInfo& info : m_graphInfos) {
            if (info.graph == graph && info.dataType == "scan") {
                graphInfo = &info;
                break;
            }
        }
        
        if (!graphInfo || !graphInfo->scanInfo) return;

        // 获取鼠标位置对应的图表坐标
        double x = graph->xAxis->pixelToCoord(event->pos().x());
        
        QString tooltipText;
        bool foundAnyPoint = false;
        // 在 xData 中查找最近的点
        if (!graphInfo->scanInfo->xData.isEmpty()) {
            // 找到最近的 x 坐标点
            int closestIndex = 0;
            double minDistance = std::numeric_limits<double>::max();
            
            for (int i = 0; i < graphInfo->scanInfo->xData.size(); ++i) {
                double distance = qAbs(graphInfo->scanInfo->xData[i] - x);
                if (distance < minDistance) {
                    minDistance = distance;
                    closestIndex = i;
                }
            }

            // 获取当前数据
            if (closestIndex < graphInfo->scanInfo->rawData.size()) {
                double currentValue = graphInfo->scanInfo->rawData[closestIndex];
                if (unitType->currentText() == "PPM") {
                    currentValue = (currentValue * m_totalPressure) * newUnitMultiplier;
                } else {
                    currentValue = currentValue * newUnitMultiplier;
                }
                tooltipText += QString("当前数据: %1\n").arg(currentValue, 0, 'e', 2);
                foundAnyPoint = true;
            }

            // 获取上一轮数据
            if (!graphInfo->scanInfo->lastRawData.isEmpty() && 
                closestIndex < graphInfo->scanInfo->lastRawData.size()) {
                double lastValue = graphInfo->scanInfo->lastRawData[closestIndex];
                if (unitType->currentText() == "PPM") {
                    lastValue = (lastValue * m_totalPressure) * newUnitMultiplier;
                } else {
                    lastValue = lastValue * newUnitMultiplier;
                }
                tooltipText += QString("上一轮数据: %1").arg(lastValue, 0, 'e', 2);
                foundAnyPoint = true;
            }
            if (foundAnyPoint) {
                // 更新追踪器标签位置和内容
                QPoint pos = event->pos();
                pos.setY(pos.y() - 10); // 稍微往上移动一点
                
                // 确保标签不会超出图表边界
                double rightSpace = graph->width() - pos.x();
                double bottomSpace = graph->height() - pos.y();
                
                Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignBottom;
                if (rightSpace < 200) { // 如果右边空间不足
                    alignment = Qt::AlignRight | Qt::AlignBottom;
                }
                if (bottomSpace < 100) { // 如果下边空间不足
                    alignment = (alignment & Qt::AlignLeft) ? 
                        (Qt::AlignLeft | Qt::AlignTop) : 
                        (Qt::AlignRight | Qt::AlignTop);
                }

                tracerLabel->position->setPixelPosition(pos);
                tracerLabel->setPositionAlignment(alignment);
                tracerLabel->setText(QString("质量数: %1\n%2")
                                   .arg(graphInfo->scanInfo->xData[closestIndex], 0, 'f', 2)
                                   .arg(tooltipText));
                tracerLabel->setVisible(true);
            } else {
                tracerLabel->setVisible(false);
            }
        } else {
            tracerLabel->setVisible(false);
        }
        
        graph->replot();
    });
    // 当鼠标离开图表时隐藏追踪器
    connect(graph, &CustomPlot::mouseLeave, this, [=]() {
        tracerLabel->setVisible(false);
        graph->replot();
    });
    // 启用交互
    graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    graph->replot();
}

void T_ScanMode::initSingleGraph(CustomPlot* graph)
{
    if (!graph) return;

    // 添加标题
    graph->plotLayout()->insertRow(0);
    graph->plotLayout()->addElement(0, 0, new QCPTextElement(graph, "单点离子流", QFont("黑体", 12, QFont::Bold)));

    // 设置图表基本属性
    graph->legend->setVisible(true);
    graph->legend->setFont(QFont("Helvetica", 9));
    graph->legend->setSelectableParts(QCPLegend::spItems);
    graph->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);

    // 隐藏Y轴
    graph->yAxis->setVisible(false);
    graph->yAxis2->setVisible(false);

    // 设置X轴
    graph->xAxis->setLabel("时间 (HH:MM:SS)");
    graph->xAxis->setTickLabelRotation(45);
    
    // 添加默认图形
    graph->addGraph();
    graph->graph(0)->setName("请添加要显示的属性");
    graph->graph(0)->setPen(QPen(Qt::gray, 1, Qt::DashLine));
    
    // 启用交互
    graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    
    graph->replot();
}

void T_ScanMode::drawScanGraph(CustomPlot* graph, const QVector<double>& x, const QVector<double>& y, 
                              const QVector<double>& lastX, const QVector<double>& lastY)
{
    if (!graph) return;

    // 确保图表有两个图形对象
    while (graph->graphCount() < 2) {
        graph->addGraph();
    }

    // 设置上一轮数据（带填充）
    if (!lastX.isEmpty() && !lastY.isEmpty()) {
        graph->graph(1)->setData(lastX, lastY);
        graph->graph(1)->setPen(QPen(Qt::blue, 1));
        graph->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));  // 设置填充颜色和透明度
        graph->graph(1)->setName("上一轮数据");
        graph->graph(1)->setVisible(true);
    }

    // 设置当前数据（不填充）
    if (!x.isEmpty() && !y.isEmpty()) {
        graph->graph(0)->setData(x, y);
        graph->graph(0)->setPen(QPen(Qt::red, 2));
        graph->graph(0)->setBrush(Qt::NoBrush);
        graph->graph(0)->setName("当前数据");
        graph->graph(0)->setVisible(true);
    }

    // 设置坐标轴范围
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::min();
    
    if (!y.isEmpty()) {
        minY = qMin(minY, *std::min_element(y.begin(), y.end()));
        maxY = qMax(maxY, *std::max_element(y.begin(), y.end()));
    }
    if (!lastY.isEmpty()) {
        minY = qMin(minY, *std::min_element(lastY.begin(), lastY.end()));
        maxY = qMax(maxY, *std::max_element(lastY.begin(), lastY.end()));
    }

    // 移除对负值的限制，添加一些边距使图表更美观
    double range = maxY - minY;
    graph->yAxis->setRange(minY - range * 0.1, maxY + range * 0.1);
    
    if (!x.isEmpty()) {
        graph->xAxis->setRange(x.first(), x.last());
    }

    graph->replot();
}
void T_ScanMode::drawSingleGraph(CustomPlot* graph, const QVector<double>& x, 
                                const QMap<QString, QVector<double>>& propertyData)
{
    if (!graph || x.isEmpty()) return;

    // 清除之前的items和图形
    graph->clearItems();
    graph->clearGraphs();

    // 隐藏Y轴
    graph->yAxis->setVisible(false);
    graph->yAxis2->setVisible(false);

    // 确保图表有足够的图形对象
    while (graph->graphCount() < propertyData.size()) {
        graph->addGraph();
    }
    
    // 设置属性之间的间距
    const double PROPERTY_SPACING = 1; // 增加间距
    const double WAVE_AMPLITUDE = 0.3;   // 减小波动幅度
    
    // 更新每个属性的数据
    int index = 0;
    
    // 清除之前的所有标签
    graph->clearItems();

    // 为每个属性分别处理数据和标签
    for (auto it = propertyData.constBegin(); it != propertyData.constEnd(); ++it) {
        const QString& propertyName = it.key();
        const QVector<double>& originalY = it.value();
        
        if (!originalY.isEmpty()) {
            // 找到第一个非零值的索引
            int startIndex = 0;
            for (int i = 0; i < originalY.size(); ++i) {
                if (originalY[i] != 0) {
                    startIndex = i;
                    break;
                }
            }
            
            QVector<double> validX, validY;
            for (int i = startIndex; i < originalY.size(); ++i) {
                validX.append(x[i]);
                validY.append(originalY[i]);
            }

            if (!validY.isEmpty()) {
                // 计算该属性的基准高度
                double baseHeight = index * PROPERTY_SPACING;
                
                // 创建显示效果
                QVector<double> displayY(validY.size());
                
                // 设置第一个点的位置
                displayY[0] = baseHeight;
                
                // 根据数据的相对变化调整后续点的位置
                for (int i = 1; i < validY.size(); ++i) {
                    if (validY[i] > validY[i-1]) {
                        displayY[i] = displayY[i-1] + WAVE_AMPLITUDE * 0.3;
                    } else if (validY[i] < validY[i-1]) {
                        displayY[i] = displayY[i-1] - WAVE_AMPLITUDE * 0.3;
                    } else {
                        displayY[i] = displayY[i-1];
                    }
                    // 限制波动范围
                    double maxDeviation = WAVE_AMPLITUDE;
                    if (displayY[i] > baseHeight + maxDeviation) {
                        displayY[i] = baseHeight + maxDeviation;
                    } else if (displayY[i] < baseHeight - maxDeviation) {
                        displayY[i] = baseHeight - maxDeviation;
                    }
                }

                graph->graph(index)->setData(validX, displayY);
                
                // 设置样式
                const PropertyConfig& config = m_availableProperties[propertyName];
                graph->graph(index)->setPen(QPen(config.color, 1.5));
                graph->graph(index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, config.color, Qt::white, 7));
                graph->graph(index)->setName(QString("%1 (%2)").arg(config.displayName).arg(config.unit));
                
                // 为当前属性创建标签
                for (int i = 0; i < validX.size(); ++i) {
                    QCPItemText* label = new QCPItemText(graph);
                    label->position->setCoords(validX[i], displayY[i] + 0.2);
                    
                    // 格式化显示值
                    double valueToDisplay = validY[i] * config.scaleFactor;  // 应用缩放因子
                    QString displayText;
                    
                    if (valueToDisplay == 0.0) {
                        displayText = "0.00";
                    } else {
                        double absValue = std::abs(valueToDisplay);
                        int magnitude = static_cast<int>(std::floor(std::log10(absValue)));
                        
                        // 根据配置和数量级决定显示格式
                        if (magnitude > -4 && magnitude < 4) {
                            displayText = QString::number(valueToDisplay, 'f', 2);
                        } else {
                            displayText = QString::number(valueToDisplay, 'e', 2);
                        }
                    }
                    
                    label->setText(displayText);
                    label->setFont(QFont("Arial", 8));
                    label->setPen(QPen(config.color));
                    label->setPositionAlignment(Qt::AlignBottom | Qt::AlignHCenter);
                }
                
                index++;
            }
        }
    }

    // 设置X轴范围（使用有效数据的范围）
    if (!x.isEmpty()) {
        const double DISPLAY_WINDOW = 30000.0; // 显示60秒的数据窗口
        double currentEndTime = x.last();
        double currentStartTime = qMax(x.first(), currentEndTime - DISPLAY_WINDOW);
        
        graph->xAxis->setRange(currentStartTime, currentEndTime);
        
        // 设置时间刻度
        QSharedPointer<QCPAxisTickerText> timeTicker(new QCPAxisTickerText);
        timeTicker->setTickCount(8);
        
        double timeRange = currentEndTime - currentStartTime;
        double tickStep = timeRange / 7.0;
        
        for (int i = 0; i <= 7; ++i) {
            double tickValue = currentStartTime + i * tickStep;
            QString timeLabel = convertTimestamp(tickValue);
            timeTicker->addTick(tickValue, timeLabel);
        }
        
        graph->xAxis->setTicker(timeTicker);
    }

    // 调整Y轴范围，确保有足够空间显示所有属性
    double totalHeight = (propertyData.size() - 1) * PROPERTY_SPACING;
    graph->yAxis->setRange(-0.5, totalHeight + 1.0);

    graph->replot();
}
void T_ScanMode::onNewDataAvailable(const QVariantMap& dataList)
{
        if (dataList.isEmpty()) return;
        const QVariantMap& data = dataList;

    for (GraphInfo& graphInfo : m_graphInfos) {
        if (graphInfo.dataType == "scan") {
            if (data.contains("measurements")) {
                QVariantList measurements = data["measurements"].toList();
                if (!measurements.isEmpty()) {
                    QVariantMap measurement = measurements.first().toMap();
                    QList<double> scanData = measurement["data"].value<QList<double>>();
                    
                    // 保存原始数据（Amps单位）
                    graphInfo.scanInfo->lastRawData = graphInfo.scanInfo->rawData;
                    graphInfo.scanInfo->rawData = scanData;
                    graphInfo.scanInfo->lastXData = graphInfo.scanInfo->xData;
                    
                    // 更新X轴数据
                    double startMass = measurement["startMass"].toDouble();
                    double ppamu = measurement["ppamu"].toDouble();
                    graphInfo.scanInfo->xData.clear();
                    for (int i = 0; i < scanData.size(); ++i) {
                        graphInfo.scanInfo->xData.append(startMass + i / ppamu);
                    }

                    QVector<double> convertedCurrentData(scanData.size());
                    QVector<double> convertedLastData;
                    // 转换当前数据
                    if (unitType->currentText() == "PPM") {
                        // PPM模式下，先乘以总压，再乘以单位乘数
                        for (int i = 0; i < scanData.size(); ++i) {
                            double rawValue = scanData[i];  // 原始值（Amps单位）
                            convertedCurrentData[i] = (rawValue * m_totalPressure) * newUnitMultiplier;
                        }
                    } else {
                        // 其他单位的正常转换
                        for (int i = 0; i < scanData.size(); ++i) {
                            convertedCurrentData[i] = scanData[i] * newUnitMultiplier;
                        }
                    }
                    
                    // 转换上一轮数据
                    if (!graphInfo.scanInfo->lastRawData.isEmpty()) {
                        convertedLastData.resize(graphInfo.scanInfo->lastRawData.size());
                        if (unitType->currentText() == "PPM") {
                            for (int i = 0; i < graphInfo.scanInfo->lastRawData.size(); ++i) {
                                double rawValue = graphInfo.scanInfo->lastRawData[i];  // 原始值（Amps单位）
                                convertedLastData[i] = (rawValue * m_totalPressure) * newUnitMultiplier;
                            }
                        } else {
                            for (int i = 0; i < graphInfo.scanInfo->lastRawData.size(); ++i) {
                                convertedLastData[i] = graphInfo.scanInfo->lastRawData[i] * newUnitMultiplier;
                            }
                        }
                    }

                    // 更新并绘制图表
                    drawScanGraph(graphInfo.graph, 
                                graphInfo.scanInfo->xData, 
                                convertedCurrentData,
                                graphInfo.scanInfo->lastXData, 
                                convertedLastData);
                }
            }
        }
        else if (graphInfo.dataType == "single" && graphInfo.singleInfo) 
        {
            const int MAX_POINTS = 100;
            bool hasNewData = false;

            // 获取时间戳（已经是相对时间）
            qint64 timestamp = data["Timestamp"].toLongLong();
            // qDebug() << "Received timestamp:" << timestamp;

            // 更新X轴数据
            if (graphInfo.singleInfo->xData.size() >= MAX_POINTS) {
                graphInfo.singleInfo->xData.removeFirst();
                for (auto& propertyValues : graphInfo.singleInfo->propertyData) {
                    if (!propertyValues.isEmpty()) {
                        propertyValues.removeFirst();
                    }
                }
            }
            graphInfo.singleInfo->xData.append(timestamp);  // 直接使用设备返回的时间戳

            // 更新每个已选属性的数据
            for (const QString& propertyName : graphInfo.singleInfo->propertyNames) {
                if (data.contains(propertyName)) {
                    double value = data[propertyName].toDouble();
                    graphInfo.singleInfo->propertyData[propertyName].append(value);
                    hasNewData = true;
                }
            }

            if (hasNewData) {
                drawSingleGraph(graphInfo.graph, 
                              graphInfo.singleInfo->xData, 
                              graphInfo.singleInfo->propertyData);
            }
        }
    }
}
// 测试函数
void T_ScanMode::testDrawGraphs()
{
    QTimer* dataTimer = new QTimer(this);
    connect(dataTimer, &QTimer::timeout, this, [this]() {
        // 创建测试数据
        QVariantMap scanData;
        
        // 模拟基本属性数据
        scanData["scanNum"] = 756;
        scanData["scanSize"] = 503;
        scanData["id"] = 756;
        scanData["FixedNumber"] = 1234567u;
        scanData["Timestamp"] = QDateTime::currentMSecsSinceEpoch();
        scanData["SystemStatus"] = -2096088174;
        
        // 模拟动态变化的数据（添加一些随机波动）
        static double time = 0;
        scanData["TotalPressure"] = 1.975969780687592e-6 * (1 + 0.1 * sin(time));
        scanData["Baseline"] = 1.5657554801860353e-14 * (1 + 0.1 * cos(time));
        scanData["TPQuad"] = 3.5283129751206843e-9 * (1 + 0.15 * sin(time * 0.5));
        scanData["TotalPressureDetectorCurrent"] = 2.8148219310963896e-9 * (1 + 0.2 * cos(time * 0.7));
        scanData["EmissionCurrent"] = 2018 + 100 * sin(time);
        scanData["AnodePotential"] = 25095 + 1000 * cos(time * 0.3);
        scanData["ElectronEnergy"] = 6988 + 500 * sin(time * 0.4);
        scanData["FocusPotential"] = 3000 + 200 * cos(time * 0.6);
        scanData["FilamentCurrent"] = 2931 + 100 * sin(time * 0.8);

        // 创建 measurements 数据
        QVariantList measurements;
        QVariantMap measurement;
        measurement["channelId"] = 4;
        measurement["stopMass"] = 50.0;
        measurement["startMass"] = 1.0;
        measurement["ppamu"] = 10.0;

        // 创建模拟的质谱数据
        QList<double> scanValues;
        for (int i = 0; i < 500; i++) {
            double mass = 1.0 + i / 10.0;  // 质量数范围 1-50
            double value = 1e-12;  // 基线

            // 添加一些特征峰
            if (abs(mass - 18) < 0.2) value = 1e-8 * (1 + 0.2 * sin(time));  // 水峰
            if (abs(mass - 28) < 0.2) value = 5e-8 * (1 + 0.15 * cos(time));  // 氮峰
            if (abs(mass - 32) < 0.2) value = 2e-8 * (1 + 0.25 * sin(time));  // 氧峰
            if (abs(mass - 44) < 0.2) value = 1e-8 * (1 + 0.3 * cos(time));   // CO2峰

            // 添加随机噪声
            value *= (1.0 + 0.1 * (rand() % 100 - 50) / 50.0);
            scanValues.append(value);
        }

        measurement["data"] = QVariant::fromValue(scanValues);
        measurements.append(measurement);
        scanData["measurements"] = measurements;

        // 发送数据
        onNewDataAvailable(scanData);

        time += 0.1;
    });

    dataTimer->start(100);  // 每100ms更新一次数据
}


void T_ScanMode::addPropertyToSingleGraph(const QString& propertyName)
{
    for (GraphInfo& graphInfo : m_graphInfos) {
        if (graphInfo.dataType == "single" && graphInfo.singleInfo) {
            if (!m_availableProperties.contains(propertyName)) {
                qWarning() << "Unknown property:" << propertyName;
                return;
            }

            if (!graphInfo.singleInfo->propertyNames.contains(propertyName)) {
                // 如果是第一个属性，清除默认图形
                if (graphInfo.singleInfo->propertyNames.isEmpty() && graphInfo.graph) {
                    graphInfo.graph->clearGraphs();
                }

                graphInfo.singleInfo->propertyNames.insert(propertyName);
                
                // 初始化数据数组
                if (!graphInfo.singleInfo->xData.isEmpty()) {
                    graphInfo.singleInfo->propertyData[propertyName] = 
                        QVector<double>(graphInfo.singleInfo->xData.size(), 0);
                } else {
                    graphInfo.singleInfo->propertyData[propertyName] = QVector<double>();
                }

                // 添加新的图形
                if (graphInfo.graph) {
                    int newIndex = graphInfo.graph->graphCount();
                    graphInfo.graph->addGraph();
                    
                    const PropertyConfig& config = m_availableProperties[propertyName];
                    graphInfo.graph->graph(newIndex)->setPen(QPen(config.color, 2));
                    graphInfo.graph->graph(newIndex)->setName(
                        QString("%1 (%2)").arg(config.displayName).arg(config.unit));

                    graphInfo.graph->replot();
                }
            }
        }
    }
}
void T_ScanMode::removePropertyFromSingleGraph(const QString& propertyName)
{
    for (GraphInfo& graphInfo : m_graphInfos) {
        if (graphInfo.dataType == "single" && graphInfo.singleInfo) {
            if (graphInfo.singleInfo->propertyNames.contains(propertyName)) {
                graphInfo.singleInfo->propertyNames.remove(propertyName);
                graphInfo.singleInfo->propertyData.remove(propertyName);
                if (graphInfo.graph) {
                    // 先清除所有items（包括标签和追踪器）
                    graphInfo.graph->clearItems();
                    // 然后清除所有图形
                    graphInfo.graph->clearGraphs();
                    
                    // 如果没有属性了，添加默认图形
                    if (graphInfo.singleInfo->propertyNames.isEmpty()) {
                        graphInfo.graph->addGraph();
                        graphInfo.graph->graph(0)->setName("请添加要显示的属性");
                        graphInfo.graph->graph(0)->setPen(QPen(Qt::gray, 1, Qt::DashLine));
                    } else {
                        // 重新创建剩余属性的图形
                        for (const QString& remainingProp : graphInfo.singleInfo->propertyNames) {
                            int newIndex = graphInfo.graph->graphCount();
                            graphInfo.graph->addGraph();
                            
                            const PropertyConfig& config = m_availableProperties[remainingProp];
                            graphInfo.graph->graph(newIndex)->setPen(QPen(config.color, 2));
                            graphInfo.graph->graph(newIndex)->setName(
                                QString("%1 (%2)").arg(config.displayName).arg(config.unit));
                            
                            if (graphInfo.singleInfo->propertyData.contains(remainingProp)) {
                                graphInfo.graph->graph(newIndex)->setData(
                                    graphInfo.singleInfo->xData, 
                                    graphInfo.singleInfo->propertyData[remainingProp]
                                );
                            }
                        }
                    }
                    
                    graphInfo.graph->replot();
                }
            }
        }
    }
}
// 同样，在clearPropertiesFromSingleGraph中也需要清理items
void T_ScanMode::clearPropertiesFromSingleGraph()
{
    for (GraphInfo& graphInfo : m_graphInfos) {
        if (graphInfo.dataType == "single") {
            graphInfo.singleInfo->propertyNames.clear();
            graphInfo.singleInfo->propertyData.clear();
            
            if (graphInfo.graph) {
                graphInfo.graph->clearItems();  // 先清除所有items
                graphInfo.graph->clearGraphs();
                graphInfo.graph->replot();
            }
        }
    }
}

void T_ScanMode::onconnectisready(bool isScanning,int startMass,int stopMass,int ppamu,int dwell)
{
    if(isScanning)
    {
        //设置扫描设备状态
        setIsScanDevice(true);
        //禁止操作扫描按钮
        setConnectToggleButtonisdisable(true);
        setScanToggleButtonisdisable(true);
        //设置数据
        setm_startMass(startMass);
        setm_stopMass(stopMass);
        setm_ppamu(ppamu);
        setm_dwell(dwell);
        //设置扫描按钮状态
        setscanButtonState(true);
        //设置扫描数据是否禁用
        setScanDataisDisable(true);

        setConnectToggleButtonisdisable(false);
        setScanToggleButtonisdisable(false);        
        //更新扫描按钮状态
        setScanButtonPressed(true);
        //如果设备已经开始扫描，则需要同步数据
        //清除扫描图表的图形
        // for (GraphInfo& graphInfo : m_graphInfos) {
        //     if (graphInfo.dataType == "scan") {
        //         graphInfo.graph->clearGraphs();
        //         graphInfo.graph->replot();
        //     }
        //     if (graphInfo.dataType == "single") {
        //         graphInfo.graph->clearGraphs();
        //         graphInfo.graph->replot();
        //     }
        // }      
        // clearAllGraphData();  
        
        ElaMessageBar::success(ElaMessageBarType::BottomLeft, "Information", "检测到设备扫描已开启!，正在同步", 3000);


    }
    else
    {
        setConnectToggleButtonisdisable(false);
        setScanToggleButtonisdisable(false);
        setIsScanDevice(false);        
                //清除扫描图表的图形
        // for (GraphInfo& graphInfo : m_graphInfos) {
        //     if (graphInfo.dataType == "scan") {
        //         graphInfo.graph->clearGraphs();
        //         graphInfo.graph->replot();
        //     }
        //     if (graphInfo.dataType == "single") {
        //         graphInfo.graph->clearGraphs();
        //         graphInfo.graph->replot();
        //     }
        // }
        // clearAllGraphData();
        
        ElaMessageBar::success(ElaMessageBarType::BottomLeft, "Information", "连接成功!", 3000);

    }
}

void T_ScanMode::onconnectclickedclose()
{
    qDebug() << "onconnectclickedclose";
    ElaMessageBar::success(ElaMessageBarType::BottomLeft, "Information", "连接已关闭!", 3000);
    setIsStopGetData(true);
    setIsScanDevice(false);
    
    setscanButtonState(false);

    setConnectToggleButtonisdisable(false);
    setScanToggleButtonisdisable(false);
}

void T_ScanMode::onscanclicked(bool isready)
{
    if(isready)
    {
        ElaMessageBar::success(ElaMessageBarType::BottomLeft, "Information", "扫描开启成功!", 3000);
        setConnectToggleButtonisdisable(false);
        setScanToggleButtonisdisable(false);
        //清除扫描图表的图形
        // for (GraphInfo& graphInfo : m_graphInfos) {
        //     if (graphInfo.dataType == "scan") {
        //         graphInfo.graph->clearGraphs();
        //         graphInfo.graph->replot();
        //     }
        //     if (graphInfo.dataType == "single") {
        //         graphInfo.graph->clearGraphs();
        //         graphInfo.graph->replot();
        //     }
        // }
        // clearAllGraphData();
    }
    else //扫描开启失败
    {
        ElaMessageBar::error(ElaMessageBarType::BottomLeft, "Information", "扫描开启失败,请关闭其他监控软件，再重新运行!", 6000);
        setIsOpenFailed(true);        
        setscanButtonState(false);
        setConnectToggleButtonisdisable(false);
        setScanToggleButtonisdisable(false);
        // clearAllGraphData();
    }
}

void T_ScanMode::onscanclickedclose(bool iscan)
{
    if(iscan == true)//用户关闭
    {
        ElaMessageBar::success(ElaMessageBarType::BottomLeft, "Information", "扫描已关闭!", 3000);
        setScanToggleButtonisdisable(false);
        setConnectToggleButtonisdisable(false);
    }
    else //其他用户关闭
    {
        ElaMessageBar::success(ElaMessageBarType::BottomLeft, "Information", "其他用户关闭了扫描!", 3000);
        setscanButtonState(false);
        setIsOtherUserClose(true);
        setScanToggleButtonisdisable(false);
        setConnectToggleButtonisdisable(false);
    }
}
//单位类型改变
void T_ScanMode::onunitMultiplierChanged(double unitMultiplier)
{
    qDebug() << "onunitMultiplierChanged";
    // 如果单位类型改变，则需要更新单位类型乘数
    newUnitMultiplier = unitMultiplier; 
    qDebug() << "newUnitMultiplier:" << newUnitMultiplier;
    // 重新计算并更新当前轮和上一轮数据
    updateGraphsWithNewUnit();
}
//清除所有图形数据
void T_ScanMode::clearAllGraphData()
{
    for (GraphInfo& info : m_graphInfos) {
        if (info.dataType == "scan") {
            // 清除扫描模式的数据
            info.scanInfo->xData.clear();
            info.scanInfo->yData.clear();
            info.scanInfo->lastXData.clear();
            info.scanInfo->lastYData.clear();
            info.scanInfo->rawData.clear();
            info.scanInfo->lastRawData.clear();
        } else if (info.dataType == "single") {
            // 清除单点模式的数据
            info.singleInfo->xData.clear();
            info.singleInfo->propertyData.clear();
        }
        
        // 重绘图表
        info.graph->replot();
    }
}

// 添加新的辅助函数来更新图表
void T_ScanMode::updateGraphsWithNewUnit()
{
    for (GraphInfo& graphInfo : m_graphInfos) {
        if (graphInfo.dataType == "scan" && graphInfo.scanInfo) {
            if (!graphInfo.scanInfo->rawData.isEmpty()) {
                QVector<double> convertedCurrentData(graphInfo.scanInfo->rawData.size());
                QVector<double> convertedLastData;
                
                // 转换当前数据
                if (unitType->currentText() == "PPM") {
                    // PPM模式下，先乘以总压，再乘以单位乘数
                    for (int i = 0; i < graphInfo.scanInfo->rawData.size(); ++i) {
                        double rawValue = graphInfo.scanInfo->rawData[i];  // 原始值（Amps单位）
                        convertedCurrentData[i] = (rawValue * m_totalPressure) * newUnitMultiplier;
                    }
                } else {
                    // 其他单位的正常转换
                    for (int i = 0; i < graphInfo.scanInfo->rawData.size(); ++i) {
                        convertedCurrentData[i] = graphInfo.scanInfo->rawData[i] * newUnitMultiplier;
                    }
                }
                
                // 转换上一轮数据
                if (!graphInfo.scanInfo->lastRawData.isEmpty()) {
                    convertedLastData.resize(graphInfo.scanInfo->lastRawData.size());
                    if (unitType->currentText() == "PPM") {
                        for (int i = 0; i < graphInfo.scanInfo->lastRawData.size(); ++i) {
                            double rawValue = graphInfo.scanInfo->lastRawData[i];  // 原始值（Amps单位）
                            convertedLastData[i] = (rawValue * m_totalPressure) * newUnitMultiplier;
                        }
                    } else {
                        for (int i = 0; i < graphInfo.scanInfo->lastRawData.size(); ++i) {
                            convertedLastData[i] = graphInfo.scanInfo->lastRawData[i] * newUnitMultiplier;
                        }
                    }
                }

                // 更新并绘制图表
                drawScanGraph(graphInfo.graph, 
                            graphInfo.scanInfo->xData, 
                            convertedCurrentData,
                            graphInfo.scanInfo->lastXData, 
                            convertedLastData);
            }
        }
    }
}
//处理测量数据
ProcessedMeasurements T_ScanMode::processMeasurements(const QVariantList& measurements) {
    ProcessedMeasurements result;
    result.tic = 0;
    
    for (const QVariant& measurementVar : measurements) {
        QVariantMap measurement = measurementVar.toMap();
        double mass = measurement["startMass"].toDouble();
        double ppamu = measurement["ppamu"].toDouble();
        QList<double> data = measurement["data"].value<QList<double>>();
        
        // 处理每个数据点
        for (int idx = 0; idx < data.size(); ++idx) {
            double val = data[idx];
            result.tic += std::abs(val);  // 计算总离子计数
            
            // 如果是整数质量点,则记录到wholeMassValues中
            if (idx % static_cast<int>(ppamu) == 0) {
                result.wholeMassValues[mass] = val;
                mass += 1.0;
            }
        }
    }
    
    return result;
}

// 总压改变信号
void T_ScanMode::ontotalPressureChanged(double totalPressure)
{
    m_totalPressure = totalPressure;
    // 如果总压为0，则设置为1
    if (std::isnan(m_totalPressure) || m_totalPressure == 0) {
        m_totalPressure = 1;
    } 
}

//时间戳转换
QString T_ScanMode::convertTimestamp(qint64 timestamp) {
    // 补零函数
    auto pad = [](int num) -> QString {
        return num < 10 ? QString("0%1").arg(num) : QString::number(num);
    };
    
    // 转换为时分秒字符串
    qint64 valueBreakdown = timestamp;
    int hours = valueBreakdown / 3600000;  // 3.6e+6 毫秒 = 1小时
    valueBreakdown -= hours * 3600000;
    
    int minutes = valueBreakdown / 60000;
    valueBreakdown -= minutes * 60000;
    
    int seconds = valueBreakdown / 1000;
    
    // 根据不同情况构建时间标签
    QString timeLabel;
    if (hours >= 1) {
        timeLabel = QString("%1:%2:%3").arg(hours).arg(pad(minutes)).arg(pad(seconds));
    } else if (minutes >= 1) {
        timeLabel = QString("00:%1:%2").arg(pad(minutes)).arg(pad(seconds));
    } else {
        timeLabel = QString("00:00:%1").arg(pad(seconds));
    }
    
    return timeLabel;
}


// 如果需要测试，可以添加以下代码：
void T_ScanMode::testTimeConversion() {
    QVector<qint64> timestamps = {468378, 471285, 474192, 477099};
    for (const auto& timestamp : timestamps) {
        qDebug() << QString("%1ms = %2").arg(timestamp).arg(convertTimestamp(timestamp));
    }
}
/*
--------------------------------
    信号转发区
--------------------------------
*/
void T_ScanMode::onscanmodeconnectclicked(QString ip, int port,bool checked)
{
    qDebug() << "onconnectclicked";
    //如果连接按钮被点击，先清理图表
    for (GraphInfo& graphInfo : m_graphInfos) {
        if (graphInfo.dataType == "scan") {
            graphInfo.graph->clearGraphs();
            graphInfo.graph->replot();
        }
        if (graphInfo.dataType == "single") {
            graphInfo.graph->clearGraphs();
            graphInfo.graph->replot();
        }
    }
    clearAllGraphData();
    emit scanmodeconnectclicked(ip, port, checked);
}

void T_ScanMode::onscanmodeclicked(QString startMass,QString stopMass,QString ppamu,QString dwell,bool scanstate)
{
    qDebug() << "onscanmodeclicked";
    //如果连接按钮被点击，先清理图表
    for (GraphInfo& graphInfo : m_graphInfos) {
        if (graphInfo.dataType == "scan") {
            graphInfo.graph->clearGraphs();
            graphInfo.graph->replot();
        }
        if (graphInfo.dataType == "single") {
            graphInfo.graph->clearGraphs();
            graphInfo.graph->replot();
        }
    }
    clearAllGraphData();

    emit scanmodeclicked(startMass, stopMass, ppamu, dwell, scanstate);
}

void T_ScanMode::onscanmodeconnectclickedclose(bool checked)
{
    qDebug() << "onscanmodeconnectclickedclose";
    //关闭的时候，清空所有single图表的属性
    _multiSelectComboBox->setCurrentSelection(QStringList());
    emit scanmodeconnectclickedclose(checked);
}

void T_ScanMode::onscanmodeclickedclose(bool checked)
{
    qDebug() << "onscanmodeclickedclose";
    _multiSelectComboBox->setCurrentSelection(QStringList());
    emit scanmodeclickedclose(checked);
}











