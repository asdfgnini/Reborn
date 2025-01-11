#include "../include/Mainwindow.h"

#include <QPushButton>  // 引入 QPushButton 头文件
#include <QVBoxLayout>   // 布局管理器
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : TaoWindow(parent),
      m_ringBuffer(new RingBuffer(10)),  // 创建大小为 10 的环形缓冲区
      m_dataFetcher(new DataFetcher(m_ringBuffer, &url1, "https://jsonplaceholder.typicode.com/posts?_limit=3")),
    url1("192.168.1.101", 80)  // 初始化 UrlGenerator 对象
{
    ui_init();



    // // 连接按钮的点击信号与槽函数
    // connect(m_button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);

    // // 数据准备线程，准备完毕启动交互线程
    // connect(&url1, &UrlGenerator::dataUpdated, this, &MainWindow::onDataUpdated);
    // url1.startUrlGenerationInThread();

    // // 连接定时器的超时信号与读取缓冲区的槽，主线程定时取数据
    // connect(&m_timer, &QTimer::timeout, this, &MainWindow::onTimeout);
    // m_timer.start(1000);  // 每秒钟检查一次缓冲区

}

void MainWindow::ui_init()
{
    initWindow();
    initEdgTaoyout();


    moveToCenter();
}
void MainWindow::initWindow()
{
    setWindowIcon(QIcon(":/ui/res/Image/ReBorn.png"));
    setWindowTitle("ReBornTool");
    resize(1200, 740);
}

void MainWindow::initEdgTaoyout()
{
    //菜单栏
    TaoMenuBar* menuBar = new TaoMenuBar(this);
    menuBar->setFixedHeight(30);
    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();

    this->setCustomWidget(TaoAppBarType::LeftArea, customWidget);
    this->setCustomWidgetMaximumWidth(500);

    menuBar->addTaoIconAction(TaoIconType::TAObars, "动作菜单");
    
    TaoMenu* iconMenu = menuBar->addMenu(TaoIconType::TAObars, "图标菜单");
    iconMenu->setMenuItemHeight(27);
    iconMenu->addTaoIconAction(TaoIconType::TAOlist_check, "排序方式", QKeySequence::SelectAll);
    iconMenu->addTaoIconAction(TaoIconType::TAOcopy, "复制");
    iconMenu->addTaoIconAction(TaoIconType::TAOvideo, "显示设置");
    iconMenu->addSeparator();
    iconMenu->addTaoIconAction(TaoIconType::TAOrotate_right, "刷新");
    iconMenu->addTaoIconAction(TaoIconType::TAOrotate_left, "撤销");
    menuBar->addSeparator();

    TaoMenu* shortCutMenu = new TaoMenu("快捷菜单(&A)", this);
    shortCutMenu->setMenuItemHeight(27);
    shortCutMenu->addTaoIconAction(TaoIconType::TAOlist_check, "排序方式", QKeySequence::Find);
    shortCutMenu->addTaoIconAction(TaoIconType::TAOcopy, "复制");
    shortCutMenu->addTaoIconAction(TaoIconType::TAOvideo, "显示设置");
    shortCutMenu->addSeparator();
    shortCutMenu->addTaoIconAction(TaoIconType::TAOrotate_right, "刷新");
    shortCutMenu->addTaoIconAction(TaoIconType::TAOrotate_left, "撤销");
    menuBar->addMenu(shortCutMenu);

    menuBar->addMenu("样例菜单(&B)")->addTaoIconAction(TaoIconType::TAOrotate_right, "样例选项");
    menuBar->addMenu("样例菜单(&C)")->addTaoIconAction(TaoIconType::TAOrotate_right, "样例选项");
    menuBar->addMenu("样例菜单(&E)")->addTaoIconAction(TaoIconType::TAOrotate_right, "样例选项");
    menuBar->addMenu("样例菜单(&F)")->addTaoIconAction(TaoIconType::TAOrotate_right, "样例选项");
    menuBar->addMenu("样例菜单(&G)")->addTaoIconAction(TaoIconType::TAOrotate_right, "样例选项");

    // //停靠窗口
    // TaoDockWidget* logDockWidget = new TaoDockWidget("日志信息", this);
    // logDockWidget->setWidget(new T_LogWidget(this));
    // this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
    // resizeDocks({logDockWidget}, {200}, Qt::Horizontal);

    // TaoDockWidget* updateDockWidget = new TaoDockWidget("更新内容", this);
    // updateDockWidget->setWidget(new T_UpdateWidget(this));
    // this->addDockWidget(Qt::RightDockWidgetArea, updateDockWidget);
    // resizeDocks({updateDockWidget}, {200}, Qt::Horizontal);


    // this->addCustomWidget(new T_BaseComponents(this));

}


MainWindow::~MainWindow() {
    // 手动释放动态分配的内存
    delete m_ringBuffer;      // 释放环形缓冲区
    delete m_dataFetcher;     // 释放数据获取器
    // 注意：QPushButton 由父类 QWidget 管理，因此不需要显式释放
    // 如果在构造时使用了 parent 参数（`this`），则 Qt 会自动删除按钮
}


void MainWindow::onTimeout() {
    qDebug() << "onTimeout is running " << QThread::currentThreadId();

    Post post;
    if (m_ringBuffer->read(post)) {
         qDebug() << "new data yet...";
    } else {
        qDebug() << "No new data yet...";
    }
}

void MainWindow::onDataUpdated() {
    // m_dataFetcher->startFetching();
}

// 按钮点击事件的槽函数
void MainWindow::onButtonClicked() {
    qDebug() << "Button clicked!";
    // 你可以在这里触发相关的数据处理操作

}
