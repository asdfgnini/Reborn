#include "rga_display.h"
#include "ui_rga_display.h"

RGA_Display::RGA_Display(QWidget *parent,API *RGA_Info,Page_Number Page_Number)
    : QWidget(parent)
    , ui(new Ui::RGA_Display)
{
    ui->setupUi(this);
    Sweep_Mode_Page *Sweep_Page=new Sweep_Mode_Page(this,RGA_Info);  //扫描界面
    RGA_Sweep_Mode_Page=Sweep_Page;
    leak_mode_page *Leak_Page=new leak_mode_page(this);   //检漏界面
    RGA_Leak_Mode_Page=Leak_Page;
    ui->stackedWidget->addWidget(RGA_Sweep_Mode_Page);
    ui->stackedWidget->addWidget(RGA_Leak_Mode_Page);
    Page_Switch(Page_Number);
}

RGA_Display::~RGA_Display()
{
    delete ui;
}


void RGA_Display::Page_Switch(Page_Number number)
{
    switch(number)
   {
    case SWEEP_PAGE_NUMBER:
        ui->stackedWidget->setCurrentWidget(RGA_Sweep_Mode_Page);
        break;

    case LEAK_PAGE_NUMBER:
        ui->stackedWidget->setCurrentWidget(RGA_Leak_Mode_Page);
        break;

   }
}
