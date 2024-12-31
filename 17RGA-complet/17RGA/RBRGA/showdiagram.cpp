#include "showdiagram.h"
#include "ui_showdiagram.h"
#include "showitem.h"
#include <QLinearGradient>

showDiagram::showDiagram(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::showDiagram)
{
    ui->setupUi(this);
    lastLine = nullptr;
    lastPoint = nullptr;
    operation=true;
    showListIndex = -1;
    textLabel = new QCPItemText(ui->customPlot);
    ui->customPlot->installEventFilter(this);
    ui->tableWidget->setColumnWidth(0,45);
    ui->tableWidget->setColumnWidth(1,120);
    ui->tableWidget->verticalHeader()->setVisible(false);
    // ui->tableWidget->horizontalHeaderItem(0)->setBackground(QBrush(QColor(23, 56, 101)));
    // ui->tableWidget->horizontalHeaderItem(1)->setBackground(QBrush(QColor(23, 56, 101)));
    ui->tableWidget->viewport()->update();
    ui->customPlot->setBackground(QBrush(QColor(10,22,46)));
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white));//轴线
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white));//刻度线
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);  // 刻度标签
    ui->customPlot->xAxis->grid()->setPen(QPen(QBrush(Qt::white),1,Qt::DotLine));
    ui->customPlot->xAxis->setLabelColor(Qt::white);      // 轴标签为白色

    ui->customPlot->yAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white));
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->grid()->setPen(QPen(QBrush(Qt::white),1,Qt::DotLine));
    ui->customPlot->yAxis->setLabelColor(Qt::white);
    ui->customPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);//缩放

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("history.db");
        //query->exec("PRAGMA key = 'ReBorN';");
        db.setUserName("ReBorN");
        db.setPassword("ReBorn");
    }
    if (!db.open()) {
        qDebug() << "open database: " << db.lastError().text();
        return;
    }
    query = new QSqlQuery(db);
    QString sql = QString("create table if not exists histories(SN text primary key,name text,resolution real,startSpectrum int,endSpectrum int,time text)");//sn加密前,name账户名,三个label显示数据
    if (!query->exec(sql)) {
        qDebug() << "create table: " << query->lastError().text();
        return;
    }
    //test
    // Data.resize(2);
    // Data[0].resize(1010);
    // Data[1].resize(1010);
    // for (int i=0; i<1010; ++i) {
    //     Data[0][i] = i/50.0 - 1; // x范围[-1,1]
    //     Data[1][i] = Data[0][i]*Data[0][i]; // y=x*x
    // }
}

showDiagram::~showDiagram()
{
    delete ui;
    db.close();
}
//-2:RGA新建name名称已存在
//-3:RGA新建SN码已存在
//-1:其他异常
int showDiagram::saveData(translate data, const QVector<QVector<double>> fromUp)//存数据库：data->histories,fromup->data.name table，data.sn:加密前
{//程序第一次运行：删除数据库，不是第一次就插入；数据库第一次建立：
    query->clear();
    QString sql="";
    db.transaction();
    if(operation){
    sql = QString("drop table IF EXISTS `%1`;").arg(data.name);
        if (!query->exec(sql)) {
            QString errorText = query->lastError().text();
            qDebug()<<errorText;
            db.rollback();
            return -1;
        }
        sql = QString("create table IF NOT EXISTS `%1`(x real, y real);").arg(data.name);
        if (!query->exec(sql)) {
            QString errorText = query->lastError().text();
            if (errorText.contains("there is already another table or index with this name", Qt::CaseInsensitive)){
                db.rollback();
                qDebug()<<__LINE__;
                return -2;
            }
            db.rollback();
            return -1;
        }
        operation = false;
    }
        sql = QString("update histories set name='%1',resolution=%2,startSpectrum=%3,endSpectrum=%4,time='%5' where SN='%6';").arg(data.name).arg(data.resolution).arg(data.startSpectrum).arg(data.endSpectrum).arg(data.time).arg(EncodedText(data.SN,"REBORn"));
        if (!query->exec(sql)) {
        // QString errorText = query->lastError().text();
        // query->clear();
        // if (errorText.contains("UNIQUE constraint failed", Qt::CaseInsensitive)){//key约束
        //     // query->prepare("DROP TABLE IF EXISTS `?`;");
        //     // query->addBindValue(data.name);
        //     // query->exec();
        //     db.rollback();
        //     return -3;
        // }
        db.rollback();
        return -1;
    }
        qDebug()<<__LINE__;

    for (int i=0;i< fromUp[0].size();i++) {
        query->prepare(QString("INSERT INTO %1 (x,y) VALUES (?,?);").arg(data.name));
        query->addBindValue(fromUp[0][i]);
        query->addBindValue(fromUp[1][i]);
        if (!query->exec()) {
            db.rollback();
            return -1;
        }
    }
    db.commit();
 qDebug()<<__LINE__;
    return 0;
}
bool showDiagram::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->customPlot) {
        if (event->type() == QEvent::Leave) {
            if(lastLine)
                lastLine->setVisible(false);
            if(lastPoint)
                lastPoint->setVisible(false);
            textLabel->setVisible(false);
            ui->customPlot->replot();
            return true;  // 返回 true，表示事件已处理
        }else if(event->type() == QEvent::Wheel){
            if(lastLine){
                QCPItemLine *line = new QCPItemLine(ui->customPlot);
                line->start->setCoords(lastLine->start->coords().x(), ui->customPlot->yAxis->range().lower);
                line->end->setCoords(lastLine->start->coords().x(), ui->customPlot->yAxis->range().upper);
                line->setPen(lastLine->pen());
                ui->customPlot->removeItem(lastLine);
                lastLine = line;
                ui->customPlot->replot();
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}
void showDiagram::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    event->accept();
}
void showDiagram::Verfiy(QString ID,bool& ver)//id：加密后
{
    query->clear();
    query->prepare("SELECT * from histories where SN = '?';");
    query->addBindValue(ID);
    query->exec();
    if (query->next()){
        ver = true;
        emit SnActively(query->value(1).toString());
    }else{
        ver = false;
        // query->clear();
        // query->prepare("INSERT INTO histories (SN,name,resolution,startSpectrum,endSpectrum) VALUES (?,?,?,?,?);");//新建记录
        // query->addBindValue(ID);
        // query->addBindValue("");
        // query->addBindValue(0);
        // query->addBindValue(0);
        // query->addBindValue(0);
        //query->exec();
        emit SnActively("histories");
    }
}
void showDiagram::insertBaseHistory(const translate &mes)//mes.sn:加密后
{
    query->clear();
    query->prepare("INSERT INTO histories (SN,name,resolution,startSpectrum,endSpectrum,time) VALUES (?,?,?,?,?,?);");//新建记录
    query->addBindValue(mes.SN);
    query->addBindValue(mes.name);
    query->addBindValue(mes.resolution);
    query->addBindValue(mes.startSpectrum);
    query->addBindValue(mes.endSpectrum);
    query->addBindValue(mes.time);
    query->exec();
}
void showDiagram::Search_In_Sqlite(QString ID, QString name,bool &ver, bool &flag)//id:加密后
{
    query->clear();
    query->prepare("SELECT SN from histories where SN = ? and name=?;");
    query->addBindValue(ID);
    query->addBindValue(name);
    query->exec();
    if (query->next())
        ver = true;
    else{
        ver = false;
    }
    flag = true;
}
void showDiagram::updateBaseHistory(const translate &mes)//mes.sn:加密后
{
    query->clear();
    query->prepare("update histories set name=?,resolution=?,startSpectrum=?,endSpectrum=?,time=? where SN=?;");
    query->addBindValue(mes.name);
    query->addBindValue(mes.resolution);
    query->addBindValue(mes.startSpectrum);
    query->addBindValue(mes.endSpectrum);
    query->addBindValue(mes.time);
    query->addBindValue(mes.SN);
    query->exec();
}
void showDiagram::updateBaseData(QString ID, const QVector<QVector<double> > &data)//id：加密后
{
    query->prepare(QString("select SN from histories where SN='%1';").arg(ID));
    query->exec();
    if(query->next()){
        query->prepare(QString("delete from %1").arg(ID));
        query->exec();
        db.transaction();
        query->clear();
        for (int i=0;i< data[0].size();i++) {
            query->prepare(QString("INSERT INTO %1 (x,y) VALUES (?,?);").arg(ID));
            query->addBindValue(data[0][i]);
            query->addBindValue(data[1][i]);
            if (!query->exec()) {
                db.rollback();
                return ;
            }
        }
        db.commit();
    }
}
//添加项槽函数：展示
void showDiagram::addListItem(translate& data,const QVector<QVector<double>>& fromUp)//data.sn:加密后
{
    showItem *item;
    item = new showItem(ui->listWidget);
    item->ItemNameChanged(data.name);
    item->scanResults = fromUp;
    QListWidgetItem *listitem = new QListWidgetItem(ui->listWidget);
    ui->listWidget->setItemWidget(listitem,item);
    listitem->setSizeHint(item->sizeHint());
    ui->listWidget->addItem(listitem);
    item->index = ui->listWidget->count()-1;
    item->setLocal(DecodedText(data.SN,"REBORn"),data.time);
    connect(item,&showItem::scanResultsClose,this,&showDiagram::qcustomplotclose);//取消画线
    connect(item,&showItem::scanResultsData,this,&showDiagram::qcustomplot_receive);//里到外画线
    connect(this,&showDiagram::closeUnique,item,&showItem::closeClick2);//唯一性约束
    connect(item,&showItem::plotColorChange,ui->customPlot,[&](QColor color){//改变颜色
        //grphSet.Color = color;
        QPen Pen = ui->customPlot->graph(0)->pen();
        Pen.setColor(color);
        ui->customPlot->graph(0)->setPen(Pen);
        ui->customPlot->replot();
    });
    connect(item,&showItem::LineShow,this,[&](bool index){//是否显示点(false显示)

        QCPScatterStyle ScatterStyle= ui->customPlot->graph(0)->scatterStyle();
        if (index)
            ScatterStyle.setShape(QCPScatterStyle::ssNone);
        else
            ScatterStyle.setShape(QCPScatterStyle::ssDisc);
        ui->customPlot->graph(0)->setScatterStyle(ScatterStyle);
        ui->customPlot->replot();
    });
    connect(item,&showItem::LineStyle,this,[&](int index){//线型0-2，Qt::SolidLine、Qt::DashLine、Qt::DashDotLine
        QPen Pen = ui->customPlot->graph(0)->pen();
        switch(index){
        case 0:Pen.setStyle(Qt::SolidLine);break;
        case 1:Pen.setStyle(Qt::DashLine);break;
        case 2:Pen.setStyle(Qt::DashDotLine);
        }
        ui->customPlot->graph(0)->setPen(Pen);
        ui->customPlot->replot();
    });
    connect(item,&showItem::LineWidth,this,[&](int index){//线粗细index0-3,width:2-5
        //grphSet.width = index+2;
        QPen Pen = ui->customPlot->graph(0)->pen();
        Pen.setWidth(index+2);
        ui->customPlot->graph(0)->setPen(Pen);
        ui->customPlot->replot();
    });
    //connect(this,&showDiagram::itemNameChanged,item,&showItem::ItemNameChanged);
   // connect(item,&showItem::NameChanged,this,&showDiagram::nameChanged);
}
void showDiagram::nameChanged(const QString Namebefore, const QString nameAfter)
{
    // query->prepare("select name from histories where name = ?;");
    // query->addBindValue(nameAfter);
    // if (query->exec()) {
    //     if(query->next()){
    //         QMessageBox::warning(this,"重命名","该RGA已存在");
    //         emit itemNameChanged(Namebefore);
    //     }
    //     // query->clear();//清除查询状态
    // }else return;
    query->clear();
    QString sql = QString("ALTER TABLE `%1` RENAME TO `%2`;").arg(Namebefore).arg(nameAfter);//崩溃
    //qDebug()<<sql;
    if (!query->exec(sql)) {
        qDebug() << "SQL execution failed: " << query->lastError().text();
        QString errorText = query->lastError().text();
        if (errorText.contains("there is already another table or index with this name", Qt::CaseInsensitive)){
            QMessageBox::warning(this,"重命名","该命名不合法");
           // emit itemNameChanged(Namebefore);
        }
        return;
    }
    //history数据
    query->clear();
    query->prepare("update histories set name = ? where name = ?;");
    query->addBindValue(nameAfter);
    query->addBindValue(Namebefore);
    if (!query->exec()) {
        qDebug() << "update histories: " << query->lastError().text();
        return;
    }
}
void showDiagram::getDataFromSqlite()
{
    //showlist供外部调用：查询几条记录就调用几次，显示label，
    QString SQL = QString("SELECT * from histories;");
    query->exec(SQL);
    //qDebug() << "Executing query for histories.";
    while (query->next()) {
        translate data;
        data.SN = query->value(0).toString();
        data.name = query->value(1).toString();
        data.resolution = query->value(2).toDouble();
        data.startSpectrum = query->value(3).toInt();
        data.endSpectrum = query->value(4).toInt();
        data.time = query->value(5).toString();
        QVector<QVector<double>> fromUp(2);
        QSqlQuery queryNow(db);
        db.transaction();
        QString sql = QString("SELECT * from `%1`;").arg(data.name);
        if (!queryNow.exec(sql)) {
            db.rollback();
            return;
        }
        while (queryNow.next()) {
            fromUp[0].push_back(queryNow.value(0).toDouble());
            fromUp[1].push_back(queryNow.value(1).toDouble());
        }
         db.commit();
        addListItem(data,fromUp);
    }
}
void showDiagram::mouseMove(QMouseEvent *event)
{
    int xPosition = event->pos().x();
    double xCoord = ui->customPlot->xAxis->pixelToCoord(xPosition);
    //找tablewidget里面的值，标注
    int closestIndex = -1;
    QTableWidgetItem *item,*itemy;
    double minDifference = std::numeric_limits<double>::max();
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        item = ui->tableWidget->item(row, 0);
        if (item) {
            double cellValue = item->text().toDouble();
            double difference = std::abs(cellValue - xCoord);
            if (difference < minDifference) {
                minDifference = difference;
                closestIndex = row;
            }
        }
    }
    item = ui->tableWidget->item(closestIndex,0);
    itemy = ui->tableWidget->item(closestIndex,1);
    //qDebug() <<"x="<<item->text().toDouble()<<"y="<<itemy->text().toDouble();

    //line
    QCPItemLine *line = new QCPItemLine(ui->customPlot);
    if(lastLine)
        ui->customPlot->removeItem(lastLine);
    lastLine =line;
    line->start->setCoords(xCoord, ui->customPlot->yAxis->range().lower);
    line->end->setCoords(xCoord, ui->customPlot->yAxis->range().upper);
    line->setPen(QPen(Qt::red,1,Qt::DotLine));
    //textlabel
    textLabel->setVisible(true);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
    textLabel->position->setCoords(item->text().toDouble(),itemy->text().toDouble());
    textLabel->setColor(QColor(Qt::white));
    textLabel->setText("y="+itemy->text());
    textLabel->setPen(QPen(Qt::darkCyan)); // 设置文本边框颜色
    textLabel->setClipToAxisRect(false);
    //point
    QCPItemTracer  *point = new QCPItemTracer (ui->customPlot);
    if(lastPoint)
        ui->customPlot->removeItem(lastPoint);
    lastPoint = point;
    // point->position->setType(QCPItemPosition::ptPlotCoords);//坐标
    point->setStyle(QCPItemTracer::tsCircle);
    point->position->setCoords(item->text().toDouble(),itemy->text().toDouble());
    point->setPen(QPen(Qt::yellow));
    point->setBrush(Qt::red);
    //point->setSize(ui->customPlot->yAxis->range().upper/10);
    ui->customPlot->replot();
}
void showDiagram::qcustomplot_receive(const QVector<QVector<double>> &data,int index,VacuumLocal LineShowSet)//运行时画线,LineShowSet.sn=加密前
{
    query->clear();
    query->prepare("SELECT * from histories where SN = ?;");
    query->addBindValue(EncodedText(LineShowSet.SN,"REBORn"));
    if(!query->exec()){
        qDebug()<<__LINE__;
        return;
    }
    while (query->next()) {
        ui->Ppamu_Value->setText(QString::number(query->value(2).toDouble())+"PPAMU");//分辨率
        ui->Start_AMU_Value->setText(QString::number(query->value(3).toInt())+"AMU");//扫描范围
        ui->Stop_AMU_Value->setText(QString::number(query->value(4).toInt())+"AMU");
    }
    if (showListIndex>=0) {
        emit closeUnique(showListIndex);
        ui->customPlot->clearGraphs();
    }
    showListIndex = index;
    // ui->tableWidget->setColumnWidth(0,60);
    // ui->tableWidget->setColumnWidth(1,60);
    ui->customPlot->addGraph();
    //real
    ui->customPlot->graph(0)->setData(data[0], data[1]);
    ui->customPlot->graph(0)->setName("真空测量");// 设置图例名称
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    //自适应坐标轴
    ui->customPlot->rescaleAxes();
    ui->customPlot->legend->setVisible(true);
    QPen Pen(LineShowSet.Color,LineShowSet.lineWidth+2,Qt::SolidLine);
    switch(LineShowSet.lineStyle){
    case 0:break;
    case 1:Pen.setStyle(Qt::DashLine);break;
    case 2:Pen.setStyle(Qt::DashDotLine);
    }
    ui->customPlot->graph(0)->setPen(Pen);//默认颜色
    if (LineShowSet.lineShowPoint)//point
        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
    else
        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    connect(ui->customPlot,&QCustomPlot::mouseMove,this,&showDiagram::mouseMove);
    ui->customPlot->legend->setBrush(QBrush(Qt::transparent));//图例透明
    ui->customPlot->legend->setTextColor(Qt::white);
    ui->customPlot->replot();

    //tablewidget
    ui->tableWidget->clearContents();//不删除标题
    //test
    // ui->tableWidget->setRowCount(x.size());

    // for (int i=0;i<x.size();i++) {
    //     ui->tableWidget->setItem(i,0, new QTableWidgetItem(QString::number(x[i])));
    //     ui->tableWidget->setItem(i,1, new QTableWidgetItem(QString::number(y[i])));
    // }
    //real
    ui->tableWidget->setRowCount(data[0].size());
    for (int i=0;i<data[0].size();i++) {
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(QString::number(data[0][i])));
        ui->tableWidget->setItem(i,1, new QTableWidgetItem(QString::number(data[1][i])));
    }
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void showDiagram::qcustomplotclose()//取消画线
{
    ui->customPlot->clearGraphs();
    showListIndex = -1;
    ui->tableWidget->clearContents();
    disconnect(ui->customPlot,&QCustomPlot::mouseMove,this,&showDiagram::mouseMove);
    ui->customPlot->replot();
}
// void showDiagram::on_pushButton_clicked()
// {
//     //addListItem(translate{0.0,0,30,""},Data);
//     getDataFromSqlite();
// }

QString showDiagram::EncodedText(QString data, QString key)
{
    //密钥长度AES_128,加密方式ECB,填充方式ZERO
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);

    //对钥匙进行QCryptographicHash加密
    QByteArray hashKey = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1);

    //对源数据加密
    QByteArray encodedText = encryption.encode(data.toUtf8(), hashKey);

    //QByteArray转QString (toBase64()不能去掉)
    QString encodeTextStr = QString::fromLatin1(encodedText.toBase64());
    //qDebug()<< "encodedText:"<< encodeTextStr;

    return encodeTextStr;
}
QString showDiagram::DecodedText(QString data, QString key)
{
    //密钥长度AES_128,加密方式ECB,填充方式ZERO
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);

    //对钥匙进行QCryptographicHash加密
    QByteArray hashKey = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha1);

    //解密
    QByteArray decodedText = encryption.decode(QByteArray::fromBase64(data.toLatin1()), hashKey);

    //QByteArray转QString
    QString decodedTextStr = QString::fromLatin1(decodedText);
    decodedTextStr.replace(QChar(0), "");
    //qDebug()<<"decodedText:"<< decodedTextStr;

    return decodedTextStr;
}



