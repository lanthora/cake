#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QIcon>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QWidgetAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("组网工具");
    setFixedSize(850, 610);
    setWindowIcon(QIcon(":/logo.ico"));

    addCentralWidget();
    addFileMenu();
    addEditMenu();
    addHelpMenu();
    addSystemTray();

    // TODO: 读配置决定是否显示主界面
    show();

    // TODO: 读配置决定是否显示后台运行通知
    backgroundNotification = true;
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 在菜单里选择退出时无需确认直接退出
    if (forceQuit) {
        event->accept();
        return;
    }

    // 点击 X 退出,有系统托盘,发送通知并以托盘形式运行
    if (QSystemTrayIcon ::isSystemTrayAvailable()) {
        if (backgroundNotification) {
            backgroundNotification = false;
            this->trayIcon->showMessage("后台运行", "进程将以托盘的形式在后台持续运行");
        }
        event->ignore();
        hide();
        return;
    }

    // 点击 X 退出,但没有系统托盘,退出前确认
    if (QMessageBox::question(this, "退出", "退出后将断开所有虚拟网络连接,确认退出?")
        == QMessageBox::No) {
        event->ignore();
        return;
    }
    event->accept();
}

void MainWindow::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    // 左键点击托盘图标隐藏或恢复主窗口
    if (reason == QSystemTrayIcon::Trigger) {
        if (isVisible() && !isMinimized()) {
            hide();
        } else {
            showNormal();
        }
    }
}

void MainWindow::directQuit()
{
    backgroundNotification = false;
    forceQuit = true;
    qApp->quit();
}

void MainWindow::addFileMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("文件");
    QAction *newAction = new QAction("新建", fileMenu);
    QAction *quitAction = new QAction("退出", fileMenu);
    connect(newAction, &QAction::triggered, detailArea, &DetailArea::reset);
    connect(quitAction, &QAction::triggered, this, &MainWindow::directQuit);

    fileMenu->addAction(newAction);
    fileMenu->addAction(quitAction);
}

void MainWindow::addEditMenu()
{
    QMenu *settingMenu = menuBar()->addMenu("编辑");
    QAction *autoStartAction = new QAction("启动选项", settingMenu);

    // TODO: 点击启动选项弹出界面,有以下三个配置:
    // 1. 是否开机启动
    // 2. 启动后是否显示主界面
    // 3. 是否显示后台运行的提示
    // 打开界面是从配置文件中读这三个值,如果还没有配置过这三个值就默认返回真.
    // 点击保存时,首先将配置写回到配置文件,再设置对应的变量.只有是否开机启动需要主动写注册表,其他变量可以再使用的时候读配置.
    // 在安装时由安装包写入了 HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
    // 开机启动: https://discretetom.github.io/posts/qt-windows-launch-on-start/

    settingMenu->addAction(autoStartAction);
}

void MainWindow::addHelpMenu()
{
    QMenu *helpMenu = menuBar()->addMenu("帮助");
    QAction *feedbackAction = new QAction("问题反馈", helpMenu);

    helpMenu->addAction(feedbackAction);
}

void MainWindow::addCentralWidget()
{
    candyList->setParent(this);
    detailArea->setParent(this);

    // 右侧保存时追加左侧列表
    detailArea->setCandyList(candyList);

    // 将列表和详细信息区域添加到主窗口
    QWidget *placeholder = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(candyList, 1);
    layout->addWidget(detailArea, 2);
    placeholder->setLayout(layout);
    setCentralWidget(placeholder);

    centralWidget()->setLayout(layout);

    // 连接列表的 itemClicked() 信号到详细信息区域的更新函数
    connect(candyList, &QListWidget::itemClicked, detailArea, &DetailArea::selectItem);
}

void MainWindow::addSystemTray()
{
    // 通过系统托盘退出
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        QMenu *trayMenu = new QMenu(this);
        QIcon quitIcon = QApplication::style()->standardIcon(QStyle::SP_TabCloseButton);
        QAction *quitAction = new QAction(quitIcon, "退出", trayMenu);
        connect(quitAction, &QAction::triggered, this, &MainWindow::directQuit);

        trayMenu->addAction(quitAction);

        trayIcon = new QSystemTrayIcon(this);
        connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onSystemTrayActivated);
        trayIcon->setIcon(QIcon(":/logo.ico"));
        trayIcon->setToolTip("组网工具");
        trayIcon->setContextMenu(trayMenu);
        trayIcon->show();
    }
}
