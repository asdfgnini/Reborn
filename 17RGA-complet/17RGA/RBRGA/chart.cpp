#include "chart.h"
#include "ui_chart.h"

Chart::Chart(QWidget *parent,API *RGA)
    : QWidget(parent)
    , ui(new Ui::Chart)
{
    ui->setupUi(this);
    API_RGA=RGA;
    connect(&create_Signal_Item_Dialog, &Create_Signal_Item_Dialog::Create_Item, this, &Chart::Create_Item);   //TCP/IP新协议设置
     connect(API_RGA, &API::Send_AMU_Signal, this, &Chart::Received_Signal);   //曲线记录
    QBrush brush(QColor(28, 28, 30));
    QCBar=new QCustomPlot(this);
    ui->chart_horizontalLayout->addWidget(QCBar);
    QCBar->setBackground(brush);

 /********************************坐标轴设置**********************************/

    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    dateTimeTicker->setDateTimeFormat("HH:mm:ss");        //设置y轴时间显示格式
    // dateTimeTicker->setTimeZone(QTimeZone::systemTimeZone()); //设置时区为本地时区
    dateTimeTicker->setTickCount(10);                         //设置坐标间隔
    QCBar->xAxis->setTicker(dateTimeTicker);              //加载坐标轴
    QCBar->xAxis->setRange(0,120);
    QCBar->xAxis->setLabel("时间");             //坐标轴名称
    QCBar->xAxis->setSubTicks(true);           //子刻度线是否可见
    QCBar->xAxis->setPadding(20);              //设置刻度与轴之间距离
    QCBar->xAxis->setTickLabelRotation(0);     //坐标文本与坐标轴角度
    QCBar->xAxis->setTickLength(0, 4);         //设置刻度内线和外线长度
    QCBar->xAxis->grid()->setVisible(true);    //网格是否可见
    QCBar->xAxis->setBasePen(QPen(Qt::white)); //基础线条颜色
    QCBar->xAxis->setTickPen(QPen(Qt::white)); //坐标刻度颜色
    QCBar->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));//网格线颜色和样式
    QCBar->xAxis->setTickLabelColor(Qt::white);  //坐标文本颜色
    QCBar->xAxis->setLabelColor(Qt::white);      //坐标轴名称文本颜色


    // prepare y axis:
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);   //对数坐标
    QCBar->yAxis->setTicker(logTicker);
    QCBar->yAxis->setScaleType(QCPAxis::stLogarithmic);
    QCBar->yAxis->setRange(-100, 100);
    QCBar->yAxis->setPadding(1); // a bit more space to the left border
    QCBar->yAxis->setLabel("Pressure");
    QCBar->yAxis->setBasePen(QPen(Qt::white));
    QCBar->yAxis->setTickPen(QPen(Qt::white));
    QCBar->yAxis->setSubTickPen(QPen(Qt::white));
    QCBar->yAxis->grid()->setSubGridVisible(true);
    QCBar->yAxis->setTickLabelColor(Qt::white);
    QCBar->yAxis->setLabelColor(Qt::white);
    QCBar->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    QCBar->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

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

    // QCBar->legend->setVisible(true);
    // QCBar->axisRect()->insetLayout()->setInsetAlignment(1, Qt::AlignBottom|Qt::AlignVCenter);
    // QCBar->legend->setBrush(QColor(255, 255, 255, 100));
    // QCBar->legend->setBorderPen(Qt::NoPen);
    // QFont legendFont = font();
    // legendFont.setPointSize(10);
    // QCBar->legend->setFont(legendFont);
    QCBar->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

}

Chart::~Chart()
{
    delete ui;
}

// void Chart::Chart_Flash()
// {
//     // QDateTime dateTime = QDateTime::currentDateTime();
//     // QDateTime twoMinutesAgo = dateTime.addSecs(-20);
//     // QCBar->xAxis->setRange(twoMinutesAgo.toSecsSinceEpoch(),dateTime.toSecsSinceEpoch());
//     // QCBar->replot();
// }

void Chart::Received_Signal(Signal_Type Type,double AMU,int Time,double value)
{
    double Time_s=Time/1000.0f;
    for(int i=0;i<Signal_List.size();i++)
    {
        if((Signal_List[i]->Setting_Info.Type==Type)&&(AMU==Signal_List[i]->Setting_Info.AMU))
        {

            Signal_List[i]->ion_current_Graph->addData(Time_s, value);
            qDebug()<<Signal_List[i]->ion_current_Graph->data()->size();
                // qDebug()<<Type<<AMU<<Time_s<<value;

        }
    }
    if(y_Auto_Flash_Switch==true)
    {
       QCBar->yAxis->rescale();
    }
    if(x_Auto_Flash_Switch==true)
    {
        double Start_Time=Time_s-10;
        if(Start_Time<0) QCBar->xAxis->setRange(0,Time_s);
        else  QCBar->xAxis->setRange(Start_Time,Time_s);

    }
    QCBar->replot();
}

void Chart::Create_Item(Signal_Type Type ,int AMU)
{

    Signal_Setting_Info Info;
    if(Type==Signal_Type::BaseLine_Type)
    {
        Info.Type=BaseLine_Type;
        Info.Name="底噪信号";
         Info.AMU=0;
    }
    else if(Type==Signal_Type::TP_Quad_Type)
    {
         Info.Type=TP_Quad_Type;
        Info.Name="四极杆压力信号";
         Info.AMU=0;
    }
    else if(Type==Signal_Type::Total_Preussure_Type)
    {

         Info.Type=Total_Preussure_Type;
        Info.Name="总压力信号";
         Info.AMU=0;
    }
    else if(Type==Signal_Type::Sweep_Type)
    {
         Info.Type=Sweep_Type;
        Info.Name="扫描信号:"+QString::number(AMU)+"m/z";
        Info.AMU=AMU;
    }
    Info.Chart_Pen.setColor(QColor(rand() % 256, rand() % 256, rand() % 256));
    Info.Chart_Pen.setWidth(2);
    Info.scatterStyle.setShape(QCPScatterStyle::ssCircle);
    Info.scatterStyle.setBrush(Info.Chart_Pen.color());
    Info.scatterStyle.setPen(Info.Chart_Pen.color());
    Info.scatterStyle.setSize(3); // 设置数据点大小
    Info.Visible=true;
    Singal_Chart_Item *Signal_Item=new Singal_Chart_Item(this,QCBar, Info);
    Signal_List.append(Signal_Item);
    QListWidgetItem *item=new QListWidgetItem;
    ui->AMU_listWidget->addItem(item);
    ui->AMU_listWidget->setItemWidget(item,Signal_Item);
    ui->AMU_listWidget->setCurrentItem(item);
    QCBar->replot();
}



void Chart::on_X_Chart_Flash_PushButton_clicked(bool checked)
{
     x_Auto_Flash_Switch=checked;
}


void Chart::on_Y_Chart_Flash_PushButton_clicked(bool checked)
{
     y_Auto_Flash_Switch=checked;
}



void Chart::on_Remove_Signal_pushButton_clicked()
{
    int current_item=ui->AMU_listWidget->currentRow();
    if(current_item!=-1)
    {
        ui->AMU_listWidget->removeItemWidget(ui->AMU_listWidget->currentItem());
        ui->AMU_listWidget->takeItem(current_item);
        QCBar->removeGraph(Signal_List[current_item]->ion_current_Graph);
        Signal_List.removeAt(current_item);
        QCBar->replot();
    }

}


void Chart::on_Add_Signal_pushButton_clicked(bool checked)
{
    create_Signal_Item_Dialog.exec();
}

void Chart::Chart_Clear()
{
    for(int i=0;i<Signal_List.size();i++)
    {
        Signal_List[i]->ion_current_Graph->data()->clear();
    }
}
