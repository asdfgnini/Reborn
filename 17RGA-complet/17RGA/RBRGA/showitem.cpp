#include "showitem.h"
#include "ui_showitem.h"

showItem::showItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::showItem)
{
    ui->setupUi(this);
    pushbutton2_check = false;
    index=-1;
    ui->label_time->setAlignment(Qt::AlignCenter);
    // ui->startPoint->setAlignment(Qt::AlignCenter);
    // ui->endPoint->setAlignment(Qt::AlignCenter);
    // ui->label_startRange->setAlignment(Qt::AlignCenter);
    // ui->label_endRange->setAlignment(Qt::AlignCenter);
    LineShowSet={Qt::blue,0,0,true,""};
    //NameBefore = ui->lineEdit_name->text();
    lineStyleIcons[0].addFile(":/new/prefix1/icon/Full_Line.png");//Qt::SolidLine
    lineStyleIcons[1].addFile(":/new/prefix1/icon/Dotted_Line.png"); //Qt::DashLine
    lineStyleIcons[2].addFile(":/new/prefix1/icon/DashDot_line.png");//Qt::DashDotLine
    lineWidthIcons[0].addFile(":/new/prefix1/icon/width_Line_one.png");
    lineWidthIcons[1].addFile(":/new/prefix1/icon/width_Line_two.png");
    lineWidthIcons[2].addFile(":/new/prefix1/icon/width_Line_three.png");
    lineWidthIcons[3].addFile(":/new/prefix1/icon/width_Line_four.png");

    ui->check_1_1->setToolTip("线型");
    ui->check_1_1->setToolTipDuration(1000);
    ui->check_1_3->setToolTip("线宽");
    ui->check_1_3->setToolTipDuration(1000);
    ui->check_1_4->setToolTip("线点");
    ui->check_1_4->setToolTipDuration(1000);
    ui->check_2->setToolTip("数据");
    ui->check_2->setToolTipDuration(1000);

    ui->check_1_1->setIcon(lineStyleIcons[0]);//lineStyle
    ui->check_1_1->setIconSize(QSize(ui->check_1_1->size()));
    ui->check_1_1->setEnabled(false);
    ui->check_1_2->setStyleSheet(QString("background-color: rgb(0,0,255);border-radius: 5px;border:2px solid blue;"));
    ui->check_1_2->setEnabled(false);
    ui->check_1_3->setIcon(lineWidthIcons[0]);//lineWidth
    ui->check_1_3->setIconSize(ui->check_1_3->size()*4/5);
    ui->check_1_3->setEnabled(false);
    ui->check_1_4->setIcon(QIcon(":/new/prefix1/icon/dot.png"));//lineShowPoint
    ui->check_1_4->setIconSize(ui->check_1_4->size());
    ui->check_1_4->setEnabled(false);
    ui->check_2->setIcon(QIcon(":/new/prefix1/icon/false.png"));
    ui->check_2->setIconSize(QSize(ui->check_2->size()*9/10));

    // connect(ui->lineEdit_name,&QLineEdit::editingFinished,this,[&](){
    //     if(NameBefore != ui->lineEdit_name->text()){
    //         emit NameChanged(NameBefore,ui->lineEdit_name->text());
    //         NameBefore = ui->lineEdit_name->text();
    //     }
    // });
    ui->lineEdit_name->setReadOnly(true);
}

showItem::~showItem()
{
    delete ui;
}

void showItem::setLocal(QString Sn,QString time)//sn：加密前
{
    ui->label_SNL->setText(Sn);
    LineShowSet.SN=Sn;
    currentDateTime = time;
    ui->label_time->setText(currentDateTime);

}

// void showItem::focusInEvent(QFocusEvent *e)//保存编辑前的内容
// {
//     if(ui->lineEdit_name->hasFocus()){
//         NameBefore = ui->lineEdit_name->text();
//     }
//     QWidget::focusInEvent(e);
// }
void showItem::on_check_1_1_clicked()//0-2,
{
    LineShowSet.lineStyle =((++LineShowSet.lineStyle)%3);
    ui->check_1_1->setIcon(lineStyleIcons[LineShowSet.lineStyle]);
    emit LineStyle(LineShowSet.lineStyle);
}
void showItem::on_check_1_2_clicked()
{
    QColor selectColor = QColorDialog::getColor();
    if(selectColor.isValid()){
        emit plotColorChange(selectColor);
        ui->check_1_2->setStyleSheet(QString("background-color: %1;border-radius: 5px;border:2px solid %2;").arg(selectColor.name()).arg(selectColor.name()));
        LineShowSet.Color = selectColor;
    }
}
void showItem::on_check_1_3_clicked()//0-3,data:2-5
{
    LineShowSet.lineWidth =((++LineShowSet.lineWidth)%4);
    ui->check_1_3->setIcon(lineWidthIcons[LineShowSet.lineWidth]);
    emit LineWidth(LineShowSet.lineWidth);
}
void showItem::on_check_1_4_clicked()
{
    LineShowSet.lineShowPoint = !LineShowSet.lineShowPoint;
    if(LineShowSet.lineShowPoint){
        ui->check_1_4->setIcon(QIcon(":/new/prefix1/icon/dot.png"));
    } else {
        ui->check_1_4->setIcon(QIcon(":/new/prefix1/icon/not_dot.png"));
    }
    emit LineShow(LineShowSet.lineShowPoint);
}

void showItem::ItemNameChanged(QString Name)
{
    // NameBefore = Name;
    ui->lineEdit_name->setText(Name);
}
void showItem::on_check_2_clicked()
{
    pushbutton2_check = !pushbutton2_check;
    if(pushbutton2_check){
        ui->check_2->setIcon(QIcon(":/new/prefix1/icon/true.png"));
        //发信号传参qvctor给showdiagram
        emit scanResultsData(scanResults,index,LineShowSet);
        ui->check_1_2->setEnabled(true);
        ui->check_1_1->setEnabled(true);
        ui->check_1_3->setEnabled(true);
        ui->check_1_4->setEnabled(true);
    }
    else{
        ui->check_2->setIcon(QIcon(":/new/prefix1/icon/false.png"));
        emit scanResultsClose();
        ui->check_1_2->setEnabled(false);
        ui->check_1_1->setEnabled(false);
        ui->check_1_3->setEnabled(false);
        ui->check_1_4->setEnabled(false);
    }
}

void showItem::closeClick2(int indexUp)
{
    if(indexUp == index){
        pushbutton2_check = false;
        ui->check_2->setIcon(QIcon(":/new/prefix1/icon/false.png"));
        ui->check_1_2->setEnabled(false);
        ui->check_1_1->setEnabled(false);
        ui->check_1_3->setEnabled(false);
        ui->check_1_4->setEnabled(false);
    }
}
// void showItem::SelectChanged(int index)
// {
//     QComboBox *Item = static_cast<QComboBox*>(sender());
//     if (Item) {
//         if (Item->property("startRange").toBool()) {
//             local.startSpectrum = Item->currentText().toInt();
//         }
//         else if (Item->property("endRange").toBool()) {
//             local.endSpectrum = Item->currentText().toInt();
//         }
//         else if (Item->property("timeRange").toBool()) {
//             QString temp = Item->currentText();
//             temp.resize(temp.length()-2);
//             local.time = temp.toInt();
//             qDebug() << local.time;
//         }
//     }
//     emit localChanged(local);
// }






