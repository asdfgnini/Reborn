#include "set_communication_dialog.h"
#include "ui_set_communication_dialog.h"

Set_communication_Dialog::Set_communication_Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Set_communication_Dialog)
{
    ui->setupUi(this);
    // this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}

Set_communication_Dialog::~Set_communication_Dialog()
{
    delete ui;
}

void Set_communication_Dialog::Refresh(QString ipAddress,int com_number)
{
    QStringList ipParts = ipAddress.split('.', Qt::SkipEmptyParts);
    if (ipParts.size() == 4)
    {
        ip1 = ipParts[0].toInt();
        ip2 = ipParts[1].toInt();
        ip3 = ipParts[2].toInt();
        ip4 = ipParts[3].toInt();
        Com_number=com_number;
        ui->IP1_spinBox->setValue(ip1);
        ui->IP2_spinBox->setValue(ip2);
        ui->IP3_spinBox->setValue(ip3);
        ui->IP4_spinBox->setValue(ip4);
        ui->Com_number_spinBox->setValue(Com_number);
    }
    else
    {
        qDebug() << "Invalid IP address format";
    }
}



void Set_communication_Dialog::on_Ack_Button_clicked()
{
    ip1=ui->IP1_spinBox->value();
    ip2=ui->IP2_spinBox->value();
    ip3=ui->IP3_spinBox->value();
    ip4=ui->IP4_spinBox->value();
    Com_number=ui->Com_number_spinBox->value();
    ip_Addr=QString::number(ip1)+"."+QString::number(ip2)+"."+QString::number(ip3)+"."+QString::number(ip4);
    emit Ask_Set_Signal();
     accept();
}


void Set_communication_Dialog::on_Cancel_Button_clicked()
{
     emit Cancel_Set_Singal();
     reject();
}

