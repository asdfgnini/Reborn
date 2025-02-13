#include "CustomPlot.h"
#include "ElaTheme.h"

CustomPlot::CustomPlot(QWidget *parent)
    :QCustomPlot(parent)
{
    setPlotColors(eTheme->getThemeMode());
    connect(eTheme, &ElaTheme::themeModeChanged, this,&CustomPlot::setPlotColors);


}
CustomPlot::~CustomPlot()
{
}

void CustomPlot::setPlotColors(ElaThemeType::ThemeMode themeMode)
{
    // 判断传入的颜色是白色还是黑色
    bool isWhite = (themeMode == ElaThemeType::Light);  // 判断是否为白色

    // 设置背景颜色
    QColor backgroundColor = isWhite ? QColor(255, 255, 255) : QColor(0x47, 0x47, 0x47);  // 白色或黑色背景
    QColor axisColor = isWhite ? QColor(0x47, 0x47, 0x47) : QColor(0, 0, 0);  // 根据背景色反转坐标轴颜色
    QColor labelColor = isWhite ? QColor(0x47, 0x47, 0x47) : QColor(0, 0, 0); // 设置坐标轴标签文字颜色
    // 设置绘图区域背景颜色
    this->axisRect()->setBackground(QBrush(backgroundColor));

    // 设置整个图表区域背景颜色
    this->setBackground(QBrush(backgroundColor));

    // 设置坐标轴的颜色
    this->xAxis->setBasePen(QPen(axisColor)); // 设置 x 轴颜色
    this->xAxis->setTickPen(QPen(axisColor)); // 设置 x 轴刻度线颜色
    this->xAxis->setSubTickPen(QPen(axisColor)); // 设置 x 轴副刻度线颜色
    this->yAxis->setBasePen(QPen(axisColor)); // 设置 y 轴颜色
    this->yAxis->setTickPen(QPen(axisColor)); // 设置 y 轴刻度线颜色
    this->yAxis->setSubTickPen(QPen(axisColor)); // 设置 y 轴副刻度线颜色

    // 如果你使用的是双轴，也可以设置第二个轴的颜色
    this->xAxis2->setBasePen(QPen(axisColor));
    this->xAxis2->setTickPen(QPen(axisColor));
    this->xAxis2->setSubTickPen(QPen(axisColor));
    this->yAxis2->setBasePen(QPen(axisColor));
    this->yAxis2->setTickPen(QPen(axisColor));
    this->yAxis2->setSubTickPen(QPen(axisColor));

    // 设置双轴标签的文字颜色
    this->xAxis2->setLabelColor(labelColor);
    this->yAxis2->setLabelColor(labelColor);
    // 重新绘制图表，应用颜色变化
    this->replot();
}



