#include "amu_chart.h"
#include "ui_amu_chart.h"

AMU_Chart::AMU_Chart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AMU_Chart)
{
    ui->setupUi(this);
    QBrush brush(QColor(28, 28, 30));
    QCBar=new QCustomPlot(this);
    QCPItemLine *line = new QCPItemLine(QCBar);
    Current_Amu_line=line;
    ui->verticalLayout_2->addWidget(QCBar);
    QCBar->setBackground(brush);
    QCPGraph *graph = new QCPGraph(QCBar->xAxis, QCBar->yAxis);
    Graph1=graph;
    graph->setPen(QPen(QColor(250, 170, 20), 2));
    graph->setName("分压");
    // QCPScatterStyle scatterStyle;
    // scatterStyle.setShape(QCPScatterStyle::ssCircle); // 设置数据点形状为圆形
    // // scatterStyle.setSize(3); // 设置数据点大小
    //  // scatterStyle.setPen(QPen(Qt::blue)); // 设置数据点边框颜色
    // // scatterStyle.setBrush(QBrush(Qt::red)); // 设置数据点填充颜色
    // graph->setScatterStyle(scatterStyle);

    /********************************坐标轴设置**********************************/

    QCBar->xAxis->setRange(0,50);
    QCBar->xAxis->setLabel("AMU");             //坐标轴名称
    QCBar->xAxis->setSubTicks(true);           //子刻度线是否可见
    QCBar->xAxis->setPadding(0);              //设置刻度与轴之间距离
    QCBar->xAxis->setTickLabelRotation(0);     //坐标文本与坐标轴角度
    QCBar->xAxis->setTickLength(0, 4);         //设置刻度内线和外线长度
    QCBar->xAxis->setSubTickLength(0, 2);         //设置刻度内线和外线长度
    QCBar->xAxis->grid()->setVisible(true);    //网格是否可见
    QCBar->xAxis->setBasePen(QPen(Qt::white)); //基础线条颜色
    QCBar->xAxis->setTickPen(QPen(Qt::white)); //坐标刻度颜色
    QCBar->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));//网格线颜色和样式
    QCBar->xAxis->setTickLabelColor(Qt::white);  //坐标文本颜色
    QCBar->xAxis->setLabelColor(Qt::white);      //坐标轴名称文本颜色
    QCBar->xAxis->setSubTickPen(QPen(Qt::white));

    // prepare y axis:
    // QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);   //对数坐标
    // QCBar->yAxis->setTicker(logTicker);
    // QCBar->yAxis->setScaleType(QCPAxis::stLogarithmic);
    QCBar->yAxis->setRange(0, 100000);
    QCBar->yAxis->setPadding(0); // a bit more space to the left border
    QCBar->yAxis->setLabel("Partial_Pressure");
    QCBar->yAxis->setBasePen(QPen(Qt::white));
    QCBar->yAxis->setTickPen(QPen(Qt::white));
    QCBar->yAxis->setSubTickPen(QPen(Qt::white));
    QCBar->yAxis->grid()->setSubGridVisible(true);
    QCBar->yAxis->setTickLabelColor(Qt::white);
    QCBar->yAxis->setLabelColor(Qt::white);
    QCBar->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    QCBar->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    QCBar->yAxis->setLabelPadding(30);
    // QDateTime dateTime1 = QDateTime::currentDateTime();
    // Tol_Pressure_PBars->addData(dateTime1.toSecsSinceEpoch(),100);
    // H20->addData(dateTime1.toSecsSinceEpoch(),100);
    // HE2->addData(dateTime1.toSecsSinceEpoch(),100);

    // QTimer *timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, &Chart::Chart_Flash);
    // timer->start(1000);

    // QTimer *timer2 = new QTimer(this);
    // connect(timer2, &QTimer::timeout, this, &Chart::Data_Flash);
    // timer2->start(1000);

    QCBar->legend->setVisible(true);
    QCBar->axisRect()->insetLayout()->setInsetAlignment(1, Qt::AlignBottom|Qt::AlignVCenter);
    QCBar->legend->setBrush(QColor(255, 255, 255, 100));
    QCBar->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    QCBar->legend->setFont(legendFont);
    QCBar->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);


    tracer = new QCPItemTracer(QCBar);
    tracer->setStyle(QCPItemTracer::tsCircle);//可以选择设置追踪光标的样式，这个是小十字，还有大十字，圆点等样式
    tracer->setPen(QPen(Qt::green,2));//设置tracer的颜色
    tracer->setBrush(Qt::red);
    tracer->setSize(5);

    QPen Target_Line_Pen;
    Target_Line_Pen.setColor(Qt::white);
    Target_Line_Pen.setWidth(1);
    Target_Line_Pen.setStyle(Qt::DashLine);

    Target_Point_Y_Line = new QCPItemLine(QCBar);
    Target_Point_Y_Line->setPen(Target_Line_Pen);
    Target_Point_Y_Line->end->setParentAnchor(tracer->position);

    Target_Point_X_Line = new QCPItemLine(QCBar);
    Target_Point_X_Line->setPen(Target_Line_Pen);
    Target_Point_X_Line->end->setParentAnchor(tracer->position);

    QPen Target_Arrow_Pen;
    Target_Line_Pen.setColor(Qt::green);
    Target_Line_Pen.setWidth(1);
    Target_Line_Pen.setStyle(Qt::SolidLine);


    Target_Point_Y_Arrow = new QCPItemLine(QCBar);
    Target_Point_Y_Arrow->setPen(Target_Line_Pen);
    Target_Point_Y_Arrow->setClipToAxisRect(false);
    Target_Point_Y_Arrow->end->setParentAnchor(Target_Point_Y_Line->start);

    Target_Point_X_Arrow = new QCPItemLine(QCBar);
    Target_Point_X_Arrow->setPen(Target_Line_Pen);
    Target_Point_X_Arrow->setClipToAxisRect(false);
    Target_Point_X_Arrow->end->setParentAnchor(Target_Point_X_Line->start);

    QFont font = QCBar->xAxis->labelFont();
    font.setBold(true);

    Target_Point_X_Value_Display = new QCPItemText(QCBar);
    Target_Point_X_Value_Display->setLayer("overlay");
    Target_Point_X_Value_Display->setClipToAxisRect(false);
    Target_Point_X_Value_Display->setPadding(QMargins(5,1,5,1));
    Target_Point_X_Value_Display->setFont(font);
    Target_Point_X_Value_Display->setColor(QCBar->xAxis->tickLabelColor());
    Target_Point_X_Value_Display->setBrush(QBrush(QColor(28, 28, 30,255)));
    Target_Point_X_Value_Display->setPen(QPen(Qt::green));
    Target_Point_X_Value_Display->setPositionAlignment(Qt::AlignHCenter |Qt::AlignTop);
    Target_Point_X_Value_Display->position->setParentAnchor(Target_Point_X_Arrow->start);

    Target_Point_Y_Value_Display = new QCPItemText(QCBar);
    Target_Point_Y_Value_Display->setLayer("overlay");
    Target_Point_Y_Value_Display->setClipToAxisRect(false);
    Target_Point_Y_Value_Display->setPadding(QMargins(5,1,5,1));
    Target_Point_Y_Value_Display->setFont(font);
    Target_Point_Y_Value_Display->setColor(QCBar->xAxis->tickLabelColor());
    Target_Point_Y_Value_Display->setBrush(QBrush(QColor(28, 28, 30,255)));
    Target_Point_Y_Value_Display->setPen(QPen(Qt::green));
    Target_Point_Y_Value_Display->setPositionAlignment(Qt::AlignVCenter |Qt::AlignRight);
    Target_Point_Y_Value_Display->position->setParentAnchor(Target_Point_Y_Arrow->start);


    connect(QCBar, SIGNAL(mouseMove(QMouseEvent*)), this,SLOT(showTracer(QMouseEvent*)));

}

AMU_Chart::~AMU_Chart()
{
    delete ui;
}

void AMU_Chart::AMU_Data_Flash(QVector<double> x_newXData,QVector<double> y_newXData,int Start_AMU,int Stop_AMU,double Current_AMU)
{

    // QCBar->xAxis->setRange(Start_AMU,Stop_AMU);
    Graph1->setData(x_newXData, y_newXData);
    // QCBar->yAxis->rescale();
    Axis_Auto_Adjust(QCBar);
    QCBar->xAxis->ticker()->setTickCount(10);
    Current_Amu_line->start->setCoords(Current_AMU, QCBar->yAxis->range().lower);
    Current_Amu_line->end->setCoords(Current_AMU, QCBar->yAxis->range().upper);
    Current_Amu_line->setPen(QPen(Qt::red));
    QCBar->replot();
    QPoint localMousePos = this->mapFromGlobal(QCursor::pos());
    Select_Points_Updates(localMousePos.x(),localMousePos.y());
    // Select_Points_Updates();
    // QCBar->xAxis->rescale();
}


void AMU_Chart::Axis_Auto_Adjust(QCustomPlot *CustomPlot)
{
    double minY = std::numeric_limits<double>::max(); // 初始化为最大可能的 double 值
    double maxY = std::numeric_limits<double>::lowest(); // 初始化为最小可能的
    for(int i=0;i<CustomPlot->graphCount();i++)
    {
        QCPGraph *Graph=CustomPlot->graph(i);
        if(Graph->dataCount()==0) continue;
        for (int i = 0; i < Graph->dataCount(); ++i)
        {
            double value = Graph->data()->at(i)->value;
            if (value < minY) {
                minY = value; // 更新最小值
            }
            if (value > maxY) {
                maxY = value; // 更新最大值
            }
        }
    }
    double  range=maxY-minY;
    double  Set_Max_Value=maxY+range*0.1;
    double  Set_Min_Value=minY-range*0.1;
    CustomPlot->yAxis->setRange(Set_Min_Value,Set_Max_Value);
}





void AMU_Chart::showTracer(QMouseEvent *event)
{
    Select_Points_Updates( event->pos().x(), event->pos().y());

}

void  AMU_Chart::Select_Points_Updates(int X_pox,int Y_pox)
{
    QPoint plotPos(X_pox,Y_pox);
    QCPAxisRect *axisRect = QCBar->axisRect();
    // qDebug()<<plotPos<<axisRect->rect()<<axisRect->rect().contains(plotPos);

    if(axisRect->rect().contains(plotPos))
    {
     double x = QCBar->xAxis->pixelToCoord(X_pox);
     double y = Y_pox;
     QPair<double, double> Current_Point;
     QSharedPointer<QCPGraphDataContainer> tmpContainer;
     tmpContainer = QCBar->graph(0)->data();
     if(tmpContainer->size()!=0)
     {
         Select_Points_SetVisible(true);
         Current_Point=Find_ShowTracer_Key(x,tmpContainer);
         // qDebug()<<x<<y<<Current_Point.first<<Current_Point.second;
         tracer->position->setCoords(Current_Point.first, Current_Point.second);
         QPointF tracer_Point=tracer->position->pixelPosition();
         tracer_Point += QPointF(0,20);
             // arrow->start->setPixelPosition(tracer_Point);
         Target_Point_Y_Line->start->setCoords(QCBar->xAxis->range().lower,Current_Point.second);
         Target_Point_X_Line->start->setCoords(Current_Point.first,QCBar->yAxis->range().lower);
         Target_Point_Y_Arrow->start->setPixelPosition(Target_Point_Y_Line->start->pixelPosition()-QPointF(5,0));
         Target_Point_X_Arrow->start->setPixelPosition(Target_Point_X_Line->start->pixelPosition()+QPointF(0,5));
         Target_Point_X_Value_Display->setText(QString::number(Current_Point.first));
         Target_Point_Y_Value_Display->setText(Scientific_notation(Current_Point.second,3));
         Target_Point_X_Value_Display->position->setCoords(0,0);
         Target_Point_Y_Value_Display->position->setCoords(0,0);
             // label->setText(tr("%1AMU:%2").arg(Current_Point.first).arg(Current_Point.second));
             // label->position->setCoords(0,0);
     }
     else
     {
        Select_Points_SetVisible(false);
     }
    }
    else
     {
        Select_Points_SetVisible(false);
     }

}


void  AMU_Chart::Select_Points_SetVisible(bool State)
{
    tracer->setVisible(State);
    Target_Point_Y_Value_Display->setVisible(State);
    Target_Point_X_Value_Display->setVisible(State);
    Target_Point_Y_Arrow->setVisible(State);
    Target_Point_X_Arrow->setVisible(State);
    Target_Point_Y_Line->setVisible(State);
    Target_Point_X_Line->setVisible(State);
}

QString  AMU_Chart::Scientific_notation(double Value,int Retain)
{
    QString Scientific_notation_text;
    Scientific_notation_text.setNum(Value,'E',Retain);
    return Scientific_notation_text;
}

QPair<double, double> AMU_Chart::Find_ShowTracer_Key(double Value, QSharedPointer<QCPGraphDataContainer> tmpContainer)
{
    double minDiff = std::numeric_limits<double>::max(); // 用于存储最小差值
    double closestKey = 0; // 假设最近的键
    double closestValue = 0; // 假设最近的值
    for (auto it = tmpContainer->constBegin(); it != tmpContainer->constEnd(); ++it)    // 遍历所有数据点
    {
        double currentKey = it->key;
        double diff = std::abs(currentKey - Value); // 计算差的绝对值
        if (diff < minDiff)
        {
            minDiff = diff; // 更新最小差值
            closestKey = currentKey; // 更新最近的键
            closestValue = it->value; // 更新最近的值
        }
    }
    return QPair<double, double>(closestKey, closestValue); // 返回最近的键和值
}
