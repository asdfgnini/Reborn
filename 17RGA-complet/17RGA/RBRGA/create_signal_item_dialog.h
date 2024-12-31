#ifndef CREATE_SIGNAL_ITEM_DIALOG_H
#define CREATE_SIGNAL_ITEM_DIALOG_H

#include <QDialog>
#include "api.h"

namespace Ui {
class Create_Signal_Item_Dialog;
}



class Create_Signal_Item_Dialog : public QDialog
{
    Q_OBJECT

signals:
    void Create_Item(Signal_Type Type,int AMU);
public:
    explicit Create_Signal_Item_Dialog(QWidget *parent = nullptr);
    ~Create_Signal_Item_Dialog();

private slots:
    void on_Signal_type_currentIndexChanged(int index);

    void on_Cancel_Button_clicked();

    void on_Ack_Button_clicked();

    void on_AMU_spinBox_valueChanged(int arg1);

private:
    Ui::Create_Signal_Item_Dialog *ui;
    Signal_Type Type;
    int AMU;
};

#endif // CREATE_SIGNAL_ITEM_DIALOG_H
