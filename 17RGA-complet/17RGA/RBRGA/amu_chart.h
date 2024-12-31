#ifndef AMU_CHART_H
#define AMU_CHART_H

#include <QWidget>
 #include<qcustomplot.h>

namespace Ui {
class AMU_Chart;
}

class AMU_Chart : public QWidget
{
    Q_OBJECT
public:

    QCustomPlot *QCBar;
    QCPBars *Bar1;
    QCPGraph *Graph1;
    QCPItemLine *Current_Amu_line;
    QCPItemTracer *tracer;
    QCPItemText *label;
    QCPItemLine *arrow;

    QCPItemText *Target_Point_Y_Value_Display;    //Y轴值显示
    QCPItemText *Target_Point_X_Value_Display;    //X轴值显示
    QCPItemLine *Target_Point_Y_Arrow; //目标点Y轴箭头
    QCPItemLine *Target_Point_X_Arrow; //目标点X轴箭头
    QCPItemLine *Target_Point_Y_Line;  //目标点Y轴指示线
    QCPItemLine *Target_Point_X_Line;  //目标点X轴指示线
public slots:
    // void Chart_Flash();
    void AMU_Data_Flash(QVector<double> x_newXData,QVector<double> y_newXData,int Start_AMU,int Stop_AMU,double Current_AMU); //数据刷新
     void showTracer(QMouseEvent *event); //曲线选点，显示某一点的数据
public:
    explicit AMU_Chart(QWidget *parent = nullptr);
    ~AMU_Chart();

    QPair<double, double> Find_ShowTracer_Key(double Value, QSharedPointer<QCPGraphDataContainer> tmpContainer);
    void Select_Points_Updates(int X_pox, int Y_pox);
    void Select_Points_SetVisible(bool State);
    QString Scientific_notation(double Value, int Retain);
    void Axis_Auto_Adjust(QCustomPlot *CustomPlot);
private:
    Ui::AMU_Chart *ui;
};

#endif // AMU_CHART_H
