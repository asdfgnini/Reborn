#include "log.h"
#include "ui_log.h"

Log::Log(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Log)
{
    ui->setupUi(this);
    ui->pushButton_log->setEnabled(false);
    ver=true;
    setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowIcon(QIcon(":/new/prefix1/icon/reborn.png"));
    ui->label_titleIcon->setPixmap(QPixmap(":/new/prefix1/icon/reborn.png"));
    ui->label_titleIcon->setScaledContents(true);
    ui->pushButton_amplify->setIcon(QIcon(":/new/prefix1/icon/amplifySma.png"));
    ui->pushButton_amplify->setIconSize(ui->pushButton_amplify->size());
    ui->pushButton_reduce->setIcon(QIcon(":/new/prefix1/icon/reduce.png"));
    ui->pushButton_reduce->setIconSize(ui->pushButton_reduce->size());
    ui->pushButton_close->setIcon(QIcon(":/new/prefix1/icon/close.png"));
    ui->pushButton_close->setIconSize(ui->pushButton_close->size());
    //url
    manager  = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Log::netReply);
    //ui->pushButton_log->setIcon(QIcon(":/Image/log.png"));

    //ui->pushButton_HistoryData->setIcon(QIcon(":/Image/history.png"));
    ui->pushButton_HistoryData->setIconSize(QSize(ui->pushButton_HistoryData->size().rwidth()/5,ui->pushButton_HistoryData->size().rheight()));
    //tcp
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::connected,this,[&](){//连接成功，进行读取url
        ui->pushButton_verification->setText("已连接");
        RGA_Communication_Stat = HTTP_OK_CONNECT;
        ui->pushButton_verification->setIcon(QIcon(":/new/prefix1/icon/TCP_Connect.png"));
        QString url = QString("http://192.168.1.101/mmsp/electronicsInfo/get");
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        manager->get(request);
    });
    connect(socket,&QTcpSocket::disconnected,this,[&](){
        ui->pushButton_verification->setText("连接");
        ui->lineEdit_SN->setText("");
        ui->lineEdit_name->setText("");
        ui->pushButton_log->setEnabled(false);
        RGA_Communication_Stat = HTTP_NO_CONNECT;
        ui->pushButton_verification->setIcon(QIcon(":/new/prefix1/icon/TCP_NoConnect.png"));
    });

    //history
    histShow = new showDiagram;
    histShow->hide();
    connect(ui->pushButton_HistoryData,SIGNAL(clicked()),histShow,SLOT(getDataFromSqlite()));
    connect(this,&Log::verfiy,histShow,&showDiagram::Verfiy);
    connect(this,&Log::searchInSqlite,histShow,&showDiagram::Search_In_Sqlite);
    connect(histShow,&showDiagram::windowClosed,this,[&](){
        this->show();
    });
    RGA_Communication_Stat = HTTP_NO_CONNECT;
    ui->pushButton_verification->setIcon(QIcon(":/new/prefix1/icon/TCP_NoConnect.png"));
    ui->pushButton_verification->setIconSize(ui->pushButton_verification->size());

}

Log::~Log()
{
    socket->close();
    delete ui;
}
// void Log::paintEvent(QPaintEvent *event){
//     QPainter painter(this);
//     painter.setBrush(QBrush(QColor(23, 56, 101)));
//     painter.drawRect(rect());
//     event->accept();
// }
void Log::on_pushButton_log_clicked()
{
    //反解密，校验，提示未连接或者未输入校验码，发送信号（含serialnumber\name）
    QString serialNumbering = ui->lineEdit_SN->text();//DecodedText(ui->lineEdit_SN->text(),"REBORn");
    bool ver1 = false;//true:含有，flase没有
    bool flag = false;//执行完成
    QString Encode = histShow->EncodedText(serialNumbering,"REBORn");
    // QEventLoop loop(this);
    // connect(histShow, &showDiagram::flagUpdated, &loop, &QEventLoop::quit);
    emit searchInSqlite(Encode,ui->lineEdit_name->text(),ver1,flag);
    //loop.exec();
    while(!flag);
    if(ver1){//核对成功,第二次登录
        emit mesAfterLog(serialNumbering,ui->lineEdit_name->text());
        qDebug()<<"登录成功";
    } else {
        if(!ver){
            bool ok;
        xx: QString text = QInputDialog::getText(this, tr("注册"),tr("请输入设备校验码（请找厂家获取）"), QLineEdit::Normal,"", &ok);
            if (ok && (text == Encode)){
                qDebug()<<text;
                histShow->insertBaseHistory(translate{0.0,0,0,Encode,ui->lineEdit_name->text()});
                ver = true;
            }
            else if(ok){
                QMessageBox::information(this,"注册","设备校验码错误，请重新输入！");
                goto xx;
            }
        }
    }
}
void Log::on_pushButton_HistoryData_clicked()
{
    this->hide();
    //histShow->getDataFromSqlite();
    histShow->show();
}
void Log::on_pushButton_verification_clicked()
{
    QString ip = ui->lineEdit_IP->text();
    QString port = ui->lineEdit_PORT->text();
    if (ip.isEmpty() || port.isEmpty()) {
        QMessageBox::warning(this, "连接", "服务器IP地址和端口不能为空!");
        return;
    }
    QRegExp regex(R"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$)");
    if(!regex.exactMatch(ip)){
        QMessageBox::warning(this,"连接","IP格式不正确，请重新输入！");
        return;
    }
    switch(RGA_Communication_Stat)
    {
    case HTTP_NO_CONNECT:{
        RGA_Communication_Stat = HTTP_CONNECTING;
        ui->pushButton_verification->setIcon(QIcon(":/new/prefix1/icon/TCP_Try.png"));
        socket->connectToHost(QHostAddress(ip), port.toUShort());
        ui->pushButton_verification->setText("连接中");
        QTimer::singleShot(3000,this,[&](){
            if(socket->state() == QAbstractSocket::ConnectingState){
                RGA_Communication_Stat = HTTP_NO_CONNECT;
                ui->pushButton_verification->setIcon(QIcon(":/new/prefix1/icon/TCP_NoConnect.png"));
                socket->abort();
                ui->pushButton_verification->setText("连接");
            }
        });
        break;
    }
    case HTTP_CONNECTING:
        // RGA_Communication_State = HTTP_NO_CONNECT;
        // ui->pushButton_verification->setIcon(QIcon(":/Image/TCP_NoConnect.png"));
        // socket->abort();
        break;

    case HTTP_OK_CONNECT:
        // RGA_Communication_State = HTTP_CONNECTING;
        // ui->pushButton_verification->setIcon(QIcon(":/Image/TCP_Try.png"));
        //RGA_Communication_State = HTTP_NO_CONNECT;
        //断开
        socket->disconnectFromHost();
        break;
    }


}
void Log::netReply(QNetworkReply *reply)
{
    QByteArray mes = reply->readAll();
    QJsonObject obj = QJsonDocument::fromJson(mes).object();

    if(obj.contains("data")){
        QJsonObject data = obj.value("data").toObject();
        serialNumber = data.value("serialNumber").toString();
        //搜索数据库中是否有serialNumber：有则输入使能输入，无则插入数据库同时加密后显示并提示保存(使能编辑)
        QString flag = "";//执行完成
        QString Encode = histShow->EncodedText(serialNumber,"REBORn");
        connect(histShow,&showDiagram::SnActively,this,[&](QString name){
            //qDebug() << name;
            if (!ver) {//首次登录
                ui->lineEdit_name->setText("MyRGA");
                //qDebug()<<__LINE__;
            }else{
                ui->lineEdit_name->setText(name);
            }
            ui->lineEdit_SN->setText(serialNumber);
            ui->pushButton_log->setEnabled(true);
        });
        emit verfiy(Encode,ver);
    }
}
void Log::on_pushButton_users_clicked()
{

}
void Log::on_pushButton_close_clicked()
{
    QWidget::close();
}
void Log::on_pushButton_amplify_clicked()
{
    if (QWidget::isMaximized())
    {
        QWidget::showNormal();//还原事件
        ui->pushButton_amplify->setIcon(QIcon(":/new/prefix1/icon/amplifySma.png"));
    }
    else
    {
        QWidget::showMaximized();//最大化事件
        ui->pushButton_amplify->setIcon(QIcon(":/new/prefix1/icon/amplifyBig.png"));
    }

}
void Log::on_pushButton_reduce_clicked()
{
    QWidget::showMinimized();
}
void Log::mousePressEvent(QMouseEvent *event)//bug:开始长按在按键上移动到其他地方的时候依旧可以拖动
{
    if(event->button() == Qt::LeftButton)
    {
        whereismouse= event->pos();
    }
    event->accept();
}
void Log::mouseMoveEvent(QMouseEvent *event)
{
    //当窗口最大化或最小化时也不进行触发
    if(QWidget::isMaximized() || QWidget::isMinimized()) {
        return;
    } else {
       if (ui->pushButton_amplify->underMouse()||ui->pushButton_close->underMouse()||ui->pushButton_reduce->underMouse()||ui->pushButton_users->underMouse()||
            ui->pushButton_HistoryData->underMouse()||ui->pushButton_log->underMouse()||ui->pushButton_verification->underMouse()){
            //左键按在按钮上期间
            return;
        }else {
            QWidget::move(QWidget::mapToGlobal(event->pos()-whereismouse));//移动    
        }
    }
    event->accept();
}
