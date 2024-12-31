#include "leak_mode_page.h"
#include "ui_leak_mode_page.h"

leak_mode_page::leak_mode_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::leak_mode_page)
{
    ui->setupUi(this);
}

leak_mode_page::~leak_mode_page()
{
    delete ui;
}
