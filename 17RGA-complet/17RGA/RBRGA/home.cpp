#include "home.h"
#include "ui_home.h"

home::home(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::home)
{
    ui->setupUi(this);
    Set_RGA_Visible(true); //开机RGA选择默认可见
}

home::~home()
{
    delete ui;
}

void home::on_Add_RGA_PushButton_clicked()
{
    RGA_item_addr Newaddr;

    RGA_item *RGA_Device=new RGA_item(this,Page_Query());  //创建RGA设备
    connect(this, &home::Switch_Page, RGA_Device->RGA_Display_Page, &RGA_Display::Page_Switch); // 切换页面槽函数连接
    QListWidgetItem *item=new QListWidgetItem;
    ui->RGA_listWidget->addItem(item);
    ui->RGA_listWidget->setItemWidget(item,RGA_Device);
    ui->Display_stackedWidget->addWidget(RGA_Device->RGA_Display_Page);
    Newaddr.WidgetItem_addr=item;
    Newaddr.Widget_addr=RGA_Device;
    RGA_addr_List.append(Newaddr);
    ui->RGA_listWidget->setCurrentItem(item);

}


void home::on_Remove_RGA_PushButton_clicked()
{
    int current_item=ui->RGA_listWidget->currentRow();
    if(current_item!=-1)
    {
        ui->RGA_listWidget->removeItemWidget(ui->RGA_listWidget->currentItem());
        ui->RGA_listWidget->takeItem(current_item);
        ui->Display_stackedWidget->removeWidget(RGA_addr_List[current_item].Widget_addr->RGA_Display_Page);
        RGA_addr_List.removeAt(current_item);
    }
}


void home::on_RGA_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow!=-1)
    {
       for(int i =0;i<RGA_addr_List.size();i++)
       {
           if(i==currentRow) RGA_addr_List[i].Widget_addr->RGA_Select(false);
            else RGA_addr_List[i].Widget_addr->RGA_Select(true);
       }
       ui->Display_stackedWidget->setCurrentIndex(currentRow);
    }

}



void home::on_Hide_RGA_PushButton_clicked()
{
    ui->RGA_Device_Page->setVisible(!ui->RGA_Device_Page->isVisible());
    ui->Show_RGA_PushButton->setVisible(!ui->RGA_Device_Page->isVisible());
}


void home::on_Show_RGA_PushButton_clicked()
{
    ui->RGA_Device_Page->setVisible(!ui->RGA_Device_Page->isVisible());
    ui->Show_RGA_PushButton->setVisible(!ui->RGA_Device_Page->isVisible());
}

void home::Set_RGA_Visible(bool Switch)
{
    ui->RGA_Device_Page->setVisible(Switch);
    ui->Show_RGA_PushButton->setVisible(!Switch);
}

Page_Number home::Page_Query()
{
    if(ui->Sweep_Mode_PushButton->isChecked()) return SWEEP_PAGE_NUMBER;
    else if(ui->Leak_Mode_PushButton->isChecked()) return LEAK_PAGE_NUMBER;
    else return SWEEP_PAGE_NUMBER;
}

void home::on_Sweep_Mode_PushButton_clicked(bool checked)
{
    if(checked) emit Switch_Page(SWEEP_PAGE_NUMBER);
}

void home::on_Leak_Mode_PushButton_clicked(bool checked)
{
    if(checked) emit Switch_Page(LEAK_PAGE_NUMBER);
}

