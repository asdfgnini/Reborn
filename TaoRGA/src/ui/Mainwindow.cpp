#include "Mainwindow.h"


#include <QPushButton>  // 引入 QPushButton 头文件
#include <QVBoxLayout>   // 布局管理器
#include <qaction.h>
#include <qlogging.h>


MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent),
    home(new T_Home(this)),
    currentIndex(0),
    currentDevice(DeviceType::Invalid),
    currentMode(ModeType::Invalid)
{
    // //初始化UI
    ui_init();
    //返回主页
    connect(this,&MainWindow::routeBackButtonClicked,this,&MainWindow::onRouteBackButtonClicked);
     // 连接主页面的设备选择信号
    connect(home, &T_Home::elaCardNavigation, this, &MainWindow::switchToPage);

}

MainWindow::~MainWindow() 
{

}

void MainWindow::onRouteBackButtonClicked()
{
    switchToWindowByIndex(0);
    setCurrentIndex(0);
    currentDevice = DeviceType::Invalid;
    currentMode = ModeType::Invalid;
    flushIconMenu(currentDevice, currentMode);
}

void MainWindow::ui_init()
{
    //初始化布局
    initWindow();

    //额外布局
    initEdgeLayout();

    //移动到中心
    moveToCenter();
}

void MainWindow::initWindow()
{
    // setIsEnableMica(true);
    // setIsCentralStackedWidgetTransparent(true);
    setWindowIcon(QIcon(":/include/Image/ReBorn.png"));
    resize(1200, 800);
    // ElaLog::getInstance()->initMessageLog(true);
    // eTheme->setThemeMode(ElaThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Compact);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);
    setWindowTitle("ReBornTool");
    // setIsStayTop(true);
    // setUserInfoCardVisible(false);
}


void MainWindow::initEdgeLayout()
{
    //菜单栏
    menuBar = new ElaMenuBar(this);
    menuBar->setFixedHeight(30);
    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();
    // this->setMenuBar(menuBar);
    this->setCustomWidget(ElaAppBarType::MiddleArea, customWidget);
    // this->setCustomWidgetMaximumWidth(120);

    // menuBar->addElaIconAction(ElaIconType::AtomSimple, "扫描模式");
    // menuBar->addElaIconAction(ElaIconType::AtomSimple, "主页1");
    // menuBar->addElaIconAction(ElaIconType::AtomSimple, "主页1");

    menuBar->setVisible(false);


    // //停靠窗口
    // logDockWidget = new ElaDockWidget("公司历史", this);
    // logDockWidget->setWidget(new T_LogWidget(this));
    // resizeDocks({logDockWidget}, {200}, Qt::Horizontal);    
    // this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);


    // ElaDockWidget* updateDockWidget = new ElaDockWidget("目前产品", this);
    // updateDockWidget->setWidget(new T_UpdateWidget(this));
    // this->addDockWidget(Qt::RightDockWidgetArea, updateDockWidget);
    // resizeDocks({updateDockWidget}, {200}, Qt::Horizontal);



    int index = addWidgetAndReturnIndex(home);
    addPageInfo(index, DeviceType::Invalid, ModeType::Invalid);
    
    rgaDevice = new RGADevice(this);   
    scanMode = rgaDevice->getm_scanMode();
    index = addWidgetAndReturnIndex(scanMode);    
    addPageInfo(index, DeviceType::RBGAseries, ModeType::ScanMode);  
   
    baseComponents = rgaDevice->getm_baseComponents();
    index = addWidgetAndReturnIndex(baseComponents);
    addPageInfo(index, DeviceType::RBGAseries, ModeType::BaseComponents);         



    // //状态栏
    // ElaStatusBar* statusBar = new ElaStatusBar(this);
    // ElaText* statusText = new ElaText("初始化成功！", this);
    // statusText->setTextPixelSize(14);
    // statusBar->addWidget(statusText);
    // this->setStatusBar(statusBar);

    printPageInfo();

}

QString MainWindow::DeviceToString(DeviceType deviceName)
{
    switch (deviceName)
    {
    case DeviceType::RBGAseries:
        return "RBGAseries";
    default:
        return "Invalid";
    }
}

QString MainWindow::ModeToString(ModeType modeName)
{
    switch (modeName)
    {
    case ModeType::ScanMode:
        return "ScanMode";
    case ModeType::BaseComponents:
        return "BaseComponents";
    default:
        return "Invalid";
    }
}

QString MainWindow::ModeToStringtoiconMenu(ModeType modeName)
{
    switch (modeName)
    {
    case ModeType::ScanMode:
        return "扫描模式";
    case ModeType::BaseComponents:
        return "捡漏模式";
    default:
        return "Invalid";
    }
}

void MainWindow::printPageInfo()
{
    for (const auto& info : pageInfoList) 
    {
        qDebug() << "Index:" << info.index << "Device:" << DeviceToString(info.deviceName) << "Mode:" << ModeToString(info.modeName);
    }
}

void MainWindow::addPageInfo(int index, DeviceType deviceName, ModeType modeName)
{
    pageInfoList.append(PageInfo(index, deviceName, modeName));
}

int MainWindow::findPageIndex(DeviceType deviceName, ModeType modeName)
{
    for (const auto& info : pageInfoList) {
        if (info.deviceName == deviceName && info.modeName == modeName) {
            return info.index;
        }
    }
    return -1;
}


void MainWindow::setCurrentIndex(int index)
{
    currentIndex = index;
}

void MainWindow::switchToPage(DeviceType deviceName, ModeType modeName)
{
    // 根据设备和模式找到对应的页面索引
    int index = findPageIndex(deviceName, modeName);
    if (index != -1) 
    {
        setCurrentIndex(index);
        switchToWindowByIndex(index);
        currentDevice = deviceName;
        currentMode = modeName;
        flushIconMenu(deviceName, modeName);
    }
    else
    {
        qDebug() << "找不到对应的页面索引";
    }
}

void MainWindow::onIconMenuActionTriggered()
{
    QAction *action = qobject_cast<QAction *>(sender());  // 获取发送信号的 QAction
    qDebug() << "Icon menu action triggered:" << action->text();
    if(action->text() == "扫描模式")
    {
        int index = findPageIndex(currentDevice, ModeType::ScanMode);
        switchToWindowByIndex(index);
        flushIconMenu(currentDevice, ModeType::ScanMode);
    }
    else if(action->text() == "捡漏模式")
    {
        int index = findPageIndex(currentDevice, ModeType::BaseComponents);
        switchToWindowByIndex(index);
        flushIconMenu(currentDevice, ModeType::BaseComponents);
    }
}

void MainWindow::flushIconMenu(DeviceType deviceName, ModeType modeName)
{
    menuBar->setVisible(false);
    menuBar->clear();
    if(deviceName != DeviceType::Invalid)
    {
        if(deviceName == DeviceType::RBGAseries)
        {
            QList<ModeType> modeNameList = rgaDevice->getm_modeName(modeName);
            for(int i = 0; i < modeNameList.size(); i++)
            {
                menuBar->addElaIconAction(ElaIconType::BoxCheck, ModeToStringtoiconMenu(modeNameList[i]));
                connect(menuBar->actions().last(), &QAction::triggered, this, &MainWindow::onIconMenuActionTriggered);
            }
            this->setCustomWidgetMaximumWidth(120 * modeNameList.size());
            menuBar->setVisible(true);
        }
        this->setRouteBackButtonEnable(true);
    }
    else
    {
        menuBar->setVisible(false);
        this->setRouteBackButtonEnable(false);
    }
}
 


