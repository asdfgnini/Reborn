#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include "include/qcustomplot.h"
#include "include/RGAData.h"
class MyWidget : public QWidget {
public:
    MyWidget(QWidget *parent = nullptr) : QWidget(parent) {
        // 创建 QCustomPlot 对象
        QCustomPlot *customPlot = new QCustomPlot(this);

        // 创建一个布局，将 QCustomPlot 添加进去
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(customPlot);

        // 生成数据：正弦、余弦和指数函数
        QVector<double> x(1000), y1(1000), y2(1000), y3(1000);
        for (int i = 0; i < 1000; ++i) {
            x[i] = i / 1000.0 * 10 - 5;  // x轴范围是 -5 到 5
            y1[i] = sin(x[i]);  // 正弦函数
            y2[i] = cos(x[i]);  // 余弦函数
            y3[i] = exp(-x[i] * x[i]);  // 高斯函数（指数衰减）
        }

        // 创建三条曲线，并添加到 QCustomPlot 中
        customPlot->addGraph();
        customPlot->graph(0)->setData(x, y1);  // 正弦曲线
        customPlot->graph(0)->setPen(QPen(Qt::blue));  // 设置颜色为蓝色
        customPlot->graph(0)->setName("sin(x)");

        customPlot->addGraph();
        customPlot->graph(1)->setData(x, y2);  // 余弦曲线
        customPlot->graph(1)->setPen(QPen(Qt::red));  // 设置颜色为红色
        customPlot->graph(1)->setName("cos(x)");

        customPlot->addGraph();
        customPlot->graph(2)->setData(x, y3);  // 高斯曲线
        customPlot->graph(2)->setPen(QPen(Qt::green));  // 设置颜色为绿色
        customPlot->graph(2)->setName("exp(-x^2)");

        // 设置坐标轴标签
        customPlot->xAxis->setLabel("X Axis");
        customPlot->yAxis->setLabel("Y Axis");

        // 设置坐标轴范围
        customPlot->xAxis->setRange(-5, 5);  // x轴范围是 -5 到 5
        customPlot->yAxis->setRange(-1.5, 1.5);  // y轴范围是 -1.5 到 1.5

        // 启用网格
        customPlot->xAxis->grid()->setSubGridVisible(true);
        customPlot->yAxis->grid()->setSubGridVisible(true);

        // 设置图例，显示图例并自动调整位置
        customPlot->legend->setVisible(true);
        customPlot->legend->setFont(QFont("Arial", 10));
        customPlot->legend->setBrush(QBrush(QColor(255, 255, 255, 200))); // 设置透明背景
        customPlot->legend->setBorderPen(QPen(Qt::black)); // 设置边框颜色

        // 启用缩放和拖拽
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

        // 允许使用鼠标滚轮进行缩放
        customPlot->setMouseTracking(true);

        // 重新绘制图表
        customPlot->replot();
    }
};


int main(int argc, char *argv[]) 
{
    // 初始化 Qt 应用程序
    QApplication app(argc, argv);
    
    qDebug() << "MAIN  is running in thread:" << QThread::currentThreadId();


    RGAData rga;
    rga.start();


   
    int result = app.exec();
    // 进入事件循环
    return result;
}
