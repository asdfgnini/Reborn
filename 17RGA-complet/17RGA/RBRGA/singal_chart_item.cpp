#include "singal_chart_item.h"
#include "ui_singal_chart_item.h"

Singal_Chart_Item::Singal_Chart_Item(QWidget *parent, QCustomPlot *qcustomplot,Signal_Setting_Info Info)
    : QWidget(parent)
    , ui(new Ui::Singal_Chart_Item)
{
    QCPGraph *Graph1=new QCPGraph(qcustomplot->xAxis,qcustomplot->yAxis);  //创建离子流曲线
    ion_current_Graph=Graph1;
    ion_current_Graph->setAdaptiveSampling(true);
    ui->setupUi(this);
    Setting_Info=Info;
    Chart_Setting(Setting_Info);
}

Singal_Chart_Item::~Singal_Chart_Item()
{
    // delete ion_current_Graph;
    delete ui;
}


void Singal_Chart_Item::Chart_Setting(Signal_Setting_Info Setting_Info)
{
    ion_current_Graph->setName(Setting_Info.Name);
    ion_current_Graph->setPen(Setting_Info.Chart_Pen);
    ion_current_Graph->setScatterStyle(Setting_Info.scatterStyle);
    ion_current_Graph->setVisible(Setting_Info.Visible);
    ui->Color_pushButton->setStyleSheet(QString("background-color: %1;border-radius: 2px;border:1px solid rgb(255, 255, 255);").arg(ion_current_Graph->pen().color().name()));
    ui->Chart_Name->setText(Setting_Info.Name);
    ui->Chart_Visible_pushButton->setChecked(Setting_Info.Visible);
}

void Singal_Chart_Item::Chart_Add_Data(double key,double value)
{
    ion_current_Graph->addData(key,value);
}


void Singal_Chart_Item::on_Color_pushButton_clicked()
{
    QColor selectedColor = QColorDialog::getColor(Qt::white, this); //调用颜色窗口
    if (selectedColor.isValid())
    {
        Setting_Info.Chart_Pen.setColor(selectedColor);
        Setting_Info.scatterStyle.setBrush(QBrush(selectedColor));
        Setting_Info.scatterStyle.setPen(QPen(selectedColor));
        ui->Color_pushButton->setStyleSheet(QString("background-color: %1;border-radius: 2px;border:1px solid rgb(255, 255, 255);").arg(selectedColor.name()));
           Chart_Setting(Setting_Info);
    }
}


void Singal_Chart_Item::on_Chart_Visible_pushButton_clicked(bool checked)
{
    Setting_Info.Visible=checked;
    Chart_Setting(Setting_Info);
}

void Singal_Chart_Item::Clear_Chart()
{
    ion_current_Graph->data()->clear();
}
