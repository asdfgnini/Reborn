#ifndef SET_COMMUNICATION_DIALOG_H
#define SET_COMMUNICATION_DIALOG_H

#include <QDialog>

namespace Ui {
class Set_communication_Dialog;
}

class Set_communication_Dialog : public QDialog
{
    Q_OBJECT


public:
    QString ip_Addr;
    int ip1;
    int ip2;
    int ip3;
    int ip4;
    int Com_number;
    explicit Set_communication_Dialog(QWidget *parent = nullptr);
    ~Set_communication_Dialog();
    void Refresh(QString ipAddress,int com_number);
signals:
    void Ask_Set_Signal();
    void Cancel_Set_Singal();

private slots:
    void on_Ack_Button_clicked();
    void on_Cancel_Button_clicked();

private:
    Ui::Set_communication_Dialog *ui;
};

#endif // SET_COMMUNICATION_DIALOG_H
