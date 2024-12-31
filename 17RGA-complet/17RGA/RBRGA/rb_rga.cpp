#include "rb_rga.h"
#include "ui_rb_rga.h"

RB_RGA::RB_RGA(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RB_RGA)
{
    ui->setupUi(this);

    // connect(&RGA, &API::RGA_Init_ok, this, &RB_RGA::RGA_Init);   //初始化完成
    // ui->Chart_verticalLayout->addWidget(&Tol_Pressure_Chart);

    // connect(&RGA, &API::TotalPressure_Flash, &Tol_Pressure_Chart, &Chart::Data_Flash);
    // connect(&RGA, &API::Partial_Pressure_Flash, &AMP_Chart, &AMU_Chart::AMU_Data_Flash);
}

RB_RGA::~RB_RGA()
{
    delete ui;
}


void RB_RGA::RGA_Init()
{

}


void RB_RGA::Scan_Setting_Display(API *addr)
{
    if(addr->RGA_Communication_State==HTTP_OK_CONNECT)
    {
        qDebug()<<123;
        ui->Scan_widget->setVisible(true);
        ui->AMU_Start_spinBox->setMaximum(ui->AMU_Stop_spinBox->value());
        ui->AMU_Start_spinBox->setMinimum(addr->RGA_SensorFilter.massMin);
        ui->AMU_Stop_spinBox->setMinimum(ui->AMU_Start_spinBox->value());
        ui->AMU_Stop_spinBox->setMaximum(addr->RGA_SensorFilter.massMax);
        QStringList dataItems = addr->RGA_ScanSetup.ppamurange.split(",");
        for (const QString &item : dataItems)
        {
            if(ui->PPamu->findText(item.trimmed())==-1) ui->PPamu->addItem(item.trimmed()); // 确保没有前后空格
        }
        for (int i = addr->RGA_SensorFilter.minDwell; i <= addr->RGA_SensorFilter.maxDwell; i *= 2)
        {
           if(ui->Dwell->findText(QString::number(i)+"ms")==-1) ui->Dwell->addItem(QString::number(i)+"ms");
        }

    }
    else
    {
        ui->Dwell->clear();
        ui->PPamu->clear();
        ui->Scan_widget->setVisible(false);
    }
}

void RB_RGA::Display(API *addr)
{
    qDebug()<<addr;
    Scan_Setting_Display(addr);
    Control_Button_Display(addr);
    Title_Display(addr);
}

void RB_RGA::Title_Display(API *addr)
{
    if(addr->RGA_Status.systemStatuss.versiscanActive==1)
    {
      ui->Start_AMU_Value->setText(QString::number(addr->RGA_Scan_Data.AMU_Start)+"AMU");
      ui->Stop_AMU_Value->setText(QString::number(addr->RGA_Scan_Data.AMU_Stop)+"AMU");
      ui->Ppamu_Value->setText(QString::number(addr->RGA_Scan_Data.Ppamu));
      ui->Tol_Pressure_Value->setText(QString::number(addr->RGA_Scan_Data.Current_TotalPressure)+"Torr");
      ui->Scan_Number_Value->setText(QString::number(addr->RGA_Scan_Data.Current_Scan_Number));
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



void RB_RGA::Control_Button_Display(API *addr)
{
    if(addr->RGA_Communication_State==HTTP_OK_CONNECT)
    {
        ui->Control_widget->setVisible(true);
        if(addr->RGA_Status.systemStatuss.versiscanActive==true)
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
        if(addr->RGA_Status.systemStatuss.requestPendingEmission==true)
        {
            ui->Emission_pushButton->setIcon(QIcon(":/new/prefix1/icon/Lamp_Operation.png"));
        }
        else if(addr->RGA_Status.systemStatuss.regulatedCurrentModeEmission==true)
        {
             ui->Emission_pushButton->setIcon(QIcon(":/new/prefix1/icon/Lamp_Opened.png"));
             ui->EM_pushButton->setVisible(true);
        }
        else if(addr->RGA_Status.systemStatuss.rfGenerator==true)
        {
              ui->Emission_pushButton->setIcon(QIcon(":/new/prefix1/icon/Lamp_Operation.png"));
        }
        else if(addr->RGA_Status.systemStatuss.regulatedCurrentModeEmission==false)
        {
            ui->Emission_pushButton->setIcon(QIcon(":/new/prefix1/icon/Lamp_Closed.png"));
            ui->EM_pushButton->setVisible(false);
        }
        if(addr->RGA_Status.systemStatuss.requestPendingElectronMultiplier==true)
        {
            ui->EM_pushButton->setIcon(QIcon(":/new/prefix1/icon/EM_Operation.png"));
        }
        else if(addr->RGA_Status.systemStatuss.electronMultiplier==true)
        {
            ui->EM_pushButton->setIcon(QIcon(":/new/prefix1/icon/EM_Opened.png"));
        }
        else if(addr->RGA_Status.systemStatuss.electronMultiplier==false)
        {
            ui->EM_pushButton->setIcon(QIcon(":/new/prefix1/icon/EM_Closed.png"));
        }

    }
    else
    {
        ui->Control_widget->setVisible(false);
    }
}



void RB_RGA::on_AMU_Start_spinBox_valueChanged(int arg1)
{
    if(arg1>=ui->AMU_Stop_spinBox->value())  ui->AMU_Start_spinBox->setValue(ui->AMU_Stop_spinBox->value());
}


void RB_RGA::on_AMU_Stop_spinBox_valueChanged(int arg1)
{
    if(arg1<=ui->AMU_Start_spinBox->value())  ui->AMU_Stop_spinBox->setValue(ui->AMU_Start_spinBox->value());
}






void RB_RGA::on_Emission_pushButton_clicked()
{
    // addr->SET_Emission();
}




void RB_RGA::on_EM_pushButton_clicked()
{
    // addr->SET_EM();
}


void RB_RGA::on_Scan_pushButton_clicked()
{
    // addr->RGA_Setting_Data.AMU_Start=ui->AMU_Start_spinBox->value();
    // addr->RGA_Setting_Data.AMU_Stop=ui->AMU_Stop_spinBox->value();
    // addr->RGA_Setting_Data.Dwell=ui->Dwell->currentText().replace("ms","").toInt();
    // qDebug()<<"dwell"<<addr->RGA_Setting_Data.Dwell;
    // addr->RGA_Setting_Data.Ppamu=ui->PPamu->currentText().toInt();
    // addr->SET_Scan();
}

