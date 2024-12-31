#include "create_signal_item_dialog.h"
#include "ui_create_signal_item_dialog.h"

Create_Signal_Item_Dialog::Create_Signal_Item_Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Create_Signal_Item_Dialog)
{
    ui->setupUi(this);
    Type=TP_Quad_Type;
    AMU=0;
    ui->Signal_type->setCurrentIndex(Type);
    ui->AMU_spinBox->setValue(AMU);

}

Create_Signal_Item_Dialog::~Create_Signal_Item_Dialog()
{
    delete ui;
}

void Create_Signal_Item_Dialog::on_Signal_type_currentIndexChanged(int index)
{
    if(index==2) ui->AMU_Select_Widget->setVisible(true);
    else ui->AMU_Select_Widget->setVisible(false);
    if(index==0) Type=BaseLine_Type;
    else if(index==1) Type=TP_Quad_Type;
    else if(index==2) Type=Sweep_Type;
    else if(index==3) Type=Total_Preussure_Type;
}


void Create_Signal_Item_Dialog::on_Cancel_Button_clicked()
{
    reject();
}


void Create_Signal_Item_Dialog::on_Ack_Button_clicked()
{

    emit Create_Item(Type, AMU);
    accept();
}


void Create_Signal_Item_Dialog::on_AMU_spinBox_valueChanged(int arg1)
{
    AMU=arg1;
}

