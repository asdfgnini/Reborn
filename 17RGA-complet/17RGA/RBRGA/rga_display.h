#ifndef RGA_DISPLAY_H
#define RGA_DISPLAY_H

#include <QWidget>
#include "sweep_mode_page.h" //扫描模式页面
#include "leak_mode_page.h"  //检漏模式页面


namespace Ui {
class RGA_Display;
}

enum Page_Number
{
    SWEEP_PAGE_NUMBER,
    LEAK_PAGE_NUMBER,

};

class RGA_Display : public QWidget
{
    Q_OBJECT

public:
    Sweep_Mode_Page *RGA_Sweep_Mode_Page;
    leak_mode_page  *RGA_Leak_Mode_Page;
    explicit RGA_Display(QWidget *parent,API *RGA_Info,Page_Number Page_Number);
    ~RGA_Display();

public slots:
    void Page_Switch(Page_Number number);
private:
    Ui::RGA_Display *ui;
};

#endif // RGA_DISPLAY_H
