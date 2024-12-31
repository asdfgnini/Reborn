#include "sweep_mode_page.h"
#include "ui_sweep_mode_page.h"

Sweep_Mode_Page::Sweep_Mode_Page(QWidget *parent,API *RGA_Info)
    : QWidget(parent)
    , ui(new Ui::Sweep_Mode_Page)

{
    API_INFO=RGA_Info;
    ui->setupUi(this);
    UI_Init();
    Chart *new_Chart= new Chart(this,RGA_Info);
    RGA_Singal_Chart=new_Chart;

    AMU_Chart *new_Chart1= new AMU_Chart(this);
    RGA_AMU_Chart=new_Chart1;

    ui->AMU_Chart_verticalLayout->addWidget(RGA_AMU_Chart);
    ui->Chart_verticalLayout->addWidget(RGA_Singal_Chart);

    connect(API_INFO, &API::Partial_Pressure_Flash, RGA_AMU_Chart, &AMU_Chart::AMU_Data_Flash);   //扫描界面数据刷新
    connect(API_INFO, &API::Communication_State_refresh, this, &Sweep_Mode_Page::UI_Refresh);   //扫描界面数据刷新
    connect(API_INFO, &API::scan_Start, RGA_Singal_Chart, &Chart::Chart_Clear);                 //扫描界面数据刷新
    connect(API_INFO, &API::RGA_Init_ok, this, &Sweep_Mode_Page::Scan_Setting_Display);   //初始化信息读取
    connect(this, &Sweep_Mode_Page::Set_Emission, RGA_Info, &API::SET_Emission);          //开关灯丝
    connect(this, &Sweep_Mode_Page::Set_Scan, RGA_Info, &API::SET_Scan);                  //开关扫描
    connect(this, &Sweep_Mode_Page::Set_EM, RGA_Info, &API::SET_EM);                      //开关电子倍增器
}

Sweep_Mode_Page::~Sweep_Mode_Page()
{
    delete ui;
}

void Sweep_Mode_Page::UI_Refresh()
{
    Page_Disabled();  //控制页面可见度
    Control_Button_Display();
    Title_Display();
}

void Sweep_Mode_Page::UI_Init()
{
      Set_Sweep_Set_Visible(true);  //扫描设置页面初始化可见
}

void Sweep_Mode_Page::Page_Disabled()
{
     if(API_INFO->RGA_Communication_State==HTTP_OK_CONNECT)
    {
        ui->Scan_Seting_widget->setDisabled(false);
        ui->Control_widget->setDisabled(false);

    }
    else
     {
        ui->Scan_Seting_widget->setDisabled(true);
        ui->Control_widget->setDisabled(true);
        ui->AMU_Start_spinBox->cleanText();
        ui->AMU_Stop_spinBox->cleanText();
        ui->Dwell->clear();
        ui->PPamu->clear();
     }

}

void Sweep_Mode_Page::Scan_Setting_Display()
{

        ui->AMU_Start_spinBox->setMaximum(ui->AMU_Stop_spinBox->value());
        ui->AMU_Start_spinBox->setMinimum(API_INFO->RGA_SensorFilter.massMin);
        ui->AMU_Stop_spinBox->setMinimum(ui->AMU_Start_spinBox->value());
        ui->AMU_Stop_spinBox->setMaximum(API_INFO->RGA_SensorFilter.massMax);

        ui->AMU_Start_spinBox->setValue(API_INFO->RGA_SensorFilter.massMin);
        ui->AMU_Stop_spinBox->setValue(API_INFO->RGA_SensorFilter.massMax);

        QStringList dataItems = API_INFO->RGA_ScanSetup.ppamurange.split(",");
        for (const QString &item : dataItems)
        {
            if(ui->PPamu->findText(item.trimmed())==-1) ui->PPamu->addItem(item.trimmed()); // 确保没有前后空格
        }
        for (int i = API_INFO->RGA_SensorFilter.minDwell; i <= API_INFO->RGA_SensorFilter.maxDwell; i *= 2)
        {
            if(ui->Dwell->findText(QString::number(i)+"ms")==-1) ui->Dwell->addItem(QString::number(i)+"ms");
        }
}



void Sweep_Mode_Page::Control_Button_Display()
{
    if(API_INFO->RGA_Communication_State==HTTP_OK_CONNECT)
    {
        if(API_INFO->RGA_Status.systemStatuss.versiscanActive==true)
        {
            ui->Scan_Seting_widget->setDisabled(true);
            ui->Scan_pushButton->setIcon(QIcon(":/new/prefix1/icon/Stop.png"));
            ui->Scan_pushButton->setText(" 停止扫描 ");
        }
        else
        {
            ui->Scan_Seting_widget->setDisabled(false);
            ui->Scan_pushButton->setIcon(QIcon(":/new/prefix1/icon/Start.png"));
            ui->Scan_pushButton->setText(" 开始扫描 ");
        }
        if(API_INFO->RGA_Status.systemStatuss.requestPendingEmission==true)
        {
            ui->Emission_pushButton->setIcon(QIcon(":/new/prefix1/icon/Lamp_Operation.png"));
        }
        else if(API_INFO->RGA_Status.systemStatuss.regulatedCurrentModeEmission==true)
        {
            ui->Emission_pushButton->setIcon(QIcon(":/new/prefix1/icon/Lamp_Opened.png"));
            ui->EM_pushButton->setVisible(true);
        }
        else if(API_INFO->RGA_Status.systemStatuss.rfGenerator==true)
        {
            ui->Emission_pushButton->setIcon(QIcon(":/new/prefix1/icon/Lamp_Operation.png"));
        }
        else if(API_INFO->RGA_Status.systemStatuss.regulatedCurrentModeEmission==false)
        {
            ui->Emission_pushButton->setIcon(QIcon(":/new/prefix1/icon/Lamp_Closed.png"));
            ui->EM_pushButton->setVisible(false);
        }
        if(API_INFO->RGA_Status.systemStatuss.requestPendingElectronMultiplier==true)
        {
            ui->EM_pushButton->setIcon(QIcon(":/new/prefix1/icon/EM_Operation.png"));
        }
        else if(API_INFO->RGA_Status.systemStatuss.electronMultiplier==true)
        {
            ui->EM_pushButton->setIcon(QIcon(":/new/prefix1/icon/EM_Opened.png"));
        }
        else if(API_INFO->RGA_Status.systemStatuss.electronMultiplier==false)
        {
            ui->EM_pushButton->setIcon(QIcon(":/new/prefix1/icon/EM_Closed.png"));
        }

    }
    else
    {
        ui->Control_widget->setDisabled(true);
    }
}

void Sweep_Mode_Page::Title_Display()
{
    if(API_INFO->RGA_Status.systemStatuss.versiscanActive==1)
    {
        ui->Start_AMU_Value->setText(QString::number(API_INFO->RGA_Scan_Data.AMU_Start)+"AMU");
        ui->Stop_AMU_Value->setText(QString::number(API_INFO->RGA_Scan_Data.AMU_Stop)+"AMU");
        ui->Ppamu_Value->setText(QString::number(API_INFO->RGA_Scan_Data.Ppamu));
        ui->Tol_Pressure_Value->setText(QString::number(API_INFO->RGA_Scan_Data.Current_TotalPressure)+"Torr");
        ui->Scan_Number_Value->setText(QString::number(API_INFO->RGA_Scan_Data.Current_Scan_Number));
    }
    else
    {
        ui->Start_AMU_Value->setText("-");
        ui->Stop_AMU_Value->setText("-");
        ui->Ppamu_Value->setText("-");
        ui->Tol_Pressure_Value->setText("-");
        ui->Scan_Number_Value->setText("-");

    }
}

void Sweep_Mode_Page::on_Emission_pushButton_clicked()
{
    emit Set_Emission();
}

void Sweep_Mode_Page::on_EM_pushButton_clicked()
{
    emit Set_EM();
}

void Sweep_Mode_Page::on_Scan_pushButton_clicked()
{
    Setting_Data RGA_Setting_Data;          //扫描配置数据
    RGA_Setting_Data.AMU_Start=ui->AMU_Start_spinBox->value();
    RGA_Setting_Data.AMU_Stop=ui->AMU_Stop_spinBox->value();
    RGA_Setting_Data.Dwell=ui->Dwell->currentText().replace("ms","").toInt();
    RGA_Setting_Data.Ppamu=ui->PPamu->currentText().toInt();
    emit Set_Scan(RGA_Setting_Data);
}

void Sweep_Mode_Page::on_AMU_Start_spinBox_valueChanged(int arg1)
{
    if(arg1>=ui->AMU_Stop_spinBox->value())  ui->AMU_Start_spinBox->setValue(ui->AMU_Stop_spinBox->value());
}


void Sweep_Mode_Page::on_AMU_Stop_spinBox_valueChanged(int arg1)
{
   if(arg1<=ui->AMU_Start_spinBox->value())  ui->AMU_Stop_spinBox->setValue(ui->AMU_Start_spinBox->value());
}


void Sweep_Mode_Page::on_Hide_Sweep_Set_PushButton_clicked()
{
    ui->Scan_widget->setVisible(false);
    ui->Show_Sweep_Set_PushButton->setVisible(true);
}


void Sweep_Mode_Page::on_Show_Sweep_Set_PushButton_clicked()
{
    ui->Scan_widget->setVisible(true);
    ui->Show_Sweep_Set_PushButton->setVisible(false);
}

void Sweep_Mode_Page::Set_Sweep_Set_Visible(bool Switch)
{
    ui->Scan_widget->setVisible(Switch);
    ui->Show_Sweep_Set_PushButton->setVisible(!Switch);
}
