#ifndef LEAK_MODE_PAGE_H
#define LEAK_MODE_PAGE_H

#include <QWidget>

namespace Ui {
class leak_mode_page;
}

class leak_mode_page : public QWidget
{
    Q_OBJECT

public:
    explicit leak_mode_page(QWidget *parent = nullptr);
    ~leak_mode_page();

private:
    Ui::leak_mode_page *ui;
};

#endif // LEAK_MODE_PAGE_H
